/**
 * @file benchmarks/nbody.cpp
 * @brief Benchmark for the nbody problem
 */

#include "bench_common.hpp"

/* The Computer Language Benchmarks Game
 * https://salsa.debian.org/benchmarksgame-team/benchmarksgame/
 *
 * contributed by Christoph Bauer
 * modified by Danny Angelo Carminati Grein
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI			  3.141592653589793
#define SOLAR_MASS	  (4 * PI * PI)
#define DAYS_PER_YEAR 365.24

typedef struct {
	double x, y, z;
	double vx, vy, vz;
	double mass;
} Planet;

constexpr int NBODIES = 5;
Planet bodies[NBODIES];

/**
 * @brief Initialize the bodies
 */
BENCHMARK_FN(initialize_bodies, {
	({
		// Sun
		bodies[0] = Planet{
			.x = 0,
			.y = 0,
			.z = 0,
			.vx = 0,
			.vy = 0,
			.vz = 0,
			.mass = SOLAR_MASS,
		};

		// Jupiter
		bodies[1] = Planet{.x = 4.84143144246472090e+00,
						   .y = -1.16032004402742839e+00,
						   .z = -1.03622044471123109e-01,
						   .vx = 1.66007664274403694e-03 * DAYS_PER_YEAR,
						   .vy = 7.69901118419740425e-03 * DAYS_PER_YEAR,
						   .vz = -6.90460016972063023e-05 * DAYS_PER_YEAR,
						   .mass = 9.54791938424326609e-04 * SOLAR_MASS};

		// Saturn
		bodies[2] = Planet{.x = 8.34336671824457987e+00,
						   .y = 4.12479856412430479e+00,
						   .z = -4.03523417114321381e-01,
						   .vx = -2.76742510726862411e-03 * DAYS_PER_YEAR,
						   .vy = 4.99852801234917238e-03 * DAYS_PER_YEAR,
						   .vz = 2.30417297573763929e-05 * DAYS_PER_YEAR,
						   .mass = 2.85885980666130812e-04 * SOLAR_MASS};

		// Uranus
		bodies[3] = Planet{.x = 1.28943695621391310e+01,
						   .y = -1.51111514016986312e+01,
						   .z = -2.23307578892655734e-01,
						   .vx = 2.96460137564761618e-03 * DAYS_PER_YEAR,
						   .vy = 2.37847173959480950e-03 * DAYS_PER_YEAR,
						   .vz = -2.96589568540237556e-05 * DAYS_PER_YEAR,
						   .mass = 4.36624404335156298e-05 * SOLAR_MASS};

		// Neptune
		bodies[4] = Planet{.x = 1.53796971148509165e+01,
						   .y = -2.59193146099879641e+01,
						   .z = 1.79258772950371181e-01,
						   .vx = 2.68067772490389322e-03 * DAYS_PER_YEAR,
						   .vy = 1.62824170038242295e-03 * DAYS_PER_YEAR,
						   .vz = -9.51592254519715870e-05 * DAYS_PER_YEAR,
						   .mass = 5.15138902046611451e-05 * SOLAR_MASS};
	});
})

BENCHMARK_FN(offset_momentum, {
	double px = 0.0;
	double py = 0.0;
	double pz = 0.0;
	for (int i = 0; i < NBODIES; i++) {
		px += bodies[i].vx * bodies[i].mass;
		py += bodies[i].vy * bodies[i].mass;
		pz += bodies[i].vz * bodies[i].mass;
	}
	bodies[0].vx = -px / SOLAR_MASS;
	bodies[0].vy = -py / SOLAR_MASS;
	bodies[0].vz = -pz / SOLAR_MASS;
})

constexpr int NB_ITERS = 1000;

BENCHMARK_FN(advance, {
	for (int iter = 0; iter < NB_ITERS; iter++) {
		for (int i = 0; i < NBODIES; i++) {
			Planet* b = &(bodies[i]);
			for (int j = i + 1; j < NBODIES; j++) {
				Planet* b2 = &(bodies[j]);
				double dx = b->x - b2->x;
				double dy = b->y - b2->y;
				double dz = b->z - b2->z;
				double distanced = dx * dx + dy * dy + dz * dz;
				double distance = sqrt(distanced);
				double mag = 0.01 / (distanced * distance);
				b->vx -= dx * b2->mass * mag;
				b->vy -= dy * b2->mass * mag;
				b->vz -= dz * b2->mass * mag;
				b2->vx += dx * b->mass * mag;
				b2->vy += dy * b->mass * mag;
				b2->vz += dz * b->mass * mag;
			}
		}
		for (int i = 0; i < NBODIES; i++) {
			Planet* b = &(bodies[i]);
			b->x += 0.01 * b->vx;
			b->y += 0.01 * b->vy;
			b->z += 0.01 * b->vz;
		}
	}
})

volatile double energy_result;
BENCHMARK_FN(energy, {
	double e = 0.0;
	for (int i = 0; i < NBODIES; i++) {
		Planet* b = &(bodies[i]);
		e += 0.5 * b->mass * (b->vx * b->vx + b->vy * b->vy + b->vz * b->vz);
		for (int j = i + 1; j < NBODIES; j++) {
			Planet* b2 = &(bodies[j]);
			double dx = b->x - b2->x;
			double dy = b->y - b2->y;
			double dz = b->z - b2->z;
			double distance = sqrt(dx * dx + dy * dy + dz * dz);
			e -= (b->mass * b2->mass) / distance;
		}
	}
	energy_result = e;
})

/* Next part was taken from
 * The Computer Language Benchmarks Game
 * https://salsa.debian.org/benchmarksgame-team/benchmarksgame/
 *
 * contributed by Miles
 */

constexpr int PAIRS = NBODIES * (NBODIES - 1) / 2;
double masses[NBODIES];
__m256d positions[NBODIES];
__m256d velocities[NBODIES];
__m256d z;
__m256d r[PAIRS + 3] __attribute__((aligned(sizeof(__m256d))));
double w[PAIRS + 3] __attribute__((aligned(sizeof(__m256d))));

BENCHMARK_FN(initialize_bodies_simd, {
	// Sun
	masses[0] = SOLAR_MASS;
	positions[0] = _mm256_set1_pd(0.0);
	velocities[0] = _mm256_set1_pd(0.0);

	// Jupiter
	masses[1] = 9.54791938424326609e-04 * SOLAR_MASS;
	positions[1] = _mm256_setr_pd(0.0, 4.84143144246472090e+00, -1.16032004402742839e+00, -1.03622044471123109e-01);
	velocities[1] = _mm256_setr_pd(
		0.0, 1.66007664274403694e-03 * DAYS_PER_YEAR, 7.69901118419740425e-03 * DAYS_PER_YEAR, -6.90460016972063023e-05 * DAYS_PER_YEAR);

	// Saturn
	masses[2] = 2.85885980666130812e-04 * SOLAR_MASS;
	positions[2] = _mm256_setr_pd(0.0, 8.34336671824457987e+00, 4.12479856412430479e+00, -4.03523417114321381e-01);
	velocities[2] = _mm256_setr_pd(
		0.0, -2.76742510726862411e-03 * DAYS_PER_YEAR, 4.99852801234917238e-03 * DAYS_PER_YEAR, 2.30417297573763929e-05 * DAYS_PER_YEAR);

	// Uranus
	masses[3] = 4.36624404335156298e-05 * SOLAR_MASS;
	positions[3] = _mm256_setr_pd(0.0, 1.28943695621391310e+01, -1.51111514016986312e+01, -2.23307578892655734e-01);
	velocities[3] = _mm256_setr_pd(
		0.0, 2.96460137564761618e-03 * DAYS_PER_YEAR, 2.37847173959480950e-03 * DAYS_PER_YEAR, -2.96589568540237556e-05 * DAYS_PER_YEAR);

	// Neptune
	masses[4] = 5.15138902046611451e-05 * SOLAR_MASS;
	positions[4] = _mm256_setr_pd(0.0, 1.53796971148509165e+01, -2.59193146099879641e+01, 1.79258772950371181e-01);
	velocities[4] = _mm256_setr_pd(
		0.0, 2.68067772490389322e-03 * DAYS_PER_YEAR, 1.62824170038242295e-03 * DAYS_PER_YEAR, -9.51592254519715870e-05 * DAYS_PER_YEAR);
})

BENCHMARK_FN(rsqrt_simd, {
	__m128 q = _mm256_cvtpd_ps(z);
	q = _mm_rsqrt_ps(q);
	__m256d x = _mm256_cvtps_pd(q);
	__m256d y = positions[0] * x * x;
	__m256d a = y * _mm256_set1_pd(0.375);
	a = a * y;
	__m256d b = y * _mm256_set1_pd(1.25);
	b = b - _mm256_set1_pd(1.875);
	y = a - b;
	x = x * y;
	z = x;
})

BENCHMARK_FN(kernel_simd, {
	for (int i = 1, k = 0; i < NBODIES; i++) {
		for (int j = 0; j < i; j++, k++) {
			r[k] = _mm256_sub_pd(positions[i], positions[j]);
		}
	}

	for (int k = 0; k < PAIRS; k += 4) {
		__m256d x0 = _mm256_mul_pd(r[k], r[k]);
		__m256d x1 = _mm256_mul_pd(r[k + 1], r[k + 1]);
		__m256d x2 = _mm256_mul_pd(r[k + 2], r[k + 2]);
		__m256d x3 = _mm256_mul_pd(r[k + 3], r[k + 3]);

		__m256d t0 = _mm256_hadd_pd(x0, x1);
		__m256d t1 = _mm256_hadd_pd(x2, x3);
		__m256d y0 = _mm256_permute2f128_pd(t0, t1, 0x21);
		__m256d y1 = _mm256_blend_pd(t0, t1, 0b1100);

		__m256d z = _mm256_add_pd(y0, y1);
		__m128 q = _mm256_cvtpd_ps(z);
		q = _mm_rsqrt_ps(q);
		__m256d x = _mm256_cvtps_pd(q);
		__m256d y = z * x * x;
		__m256d a = y * _mm256_set1_pd(0.375);
		a = a * y;
		__m256d b = y * _mm256_set1_pd(1.25);
		b = b - _mm256_set1_pd(1.875);
		z = a - b;
		z = x * z;
		_mm256_store_pd(w + k, z);
	}
})

BENCHMARK_FN(energy_simd, {
	double e = 0.0;

	__m256d r[PAIRS + 3];
	double w[PAIRS + 3] __attribute__((aligned(sizeof(__m256d))));

	r[NBODIES] = _mm256_set1_pd(0.0);
	r[NBODIES + 1] = _mm256_set1_pd(0.0);
	r[NBODIES + 2] = _mm256_set1_pd(0.0);

	for (int k = 0; k < NBODIES; k++) {
		r[k] = velocities[k] * velocities[k];
	}

	for (int k = 0; k < NBODIES; k += 4) {
		__m256d t0 = _mm256_hadd_pd(r[k], r[k + 1]);
		__m256d t1 = _mm256_hadd_pd(r[k + 2], r[k + 3]);
		__m256d y0 = _mm256_permute2f128_pd(t0, t1, 0x21);
		__m256d y1 = _mm256_blend_pd(t0, t1, 0b1100);

		__m256d z = y0 + y1;
		_mm256_store_pd(w + k, z);
	}

	for (int k = 0; k < NBODIES; k++) {
		e += 0.5 * masses[k] * w[k];
	}

	r[PAIRS] = _mm256_set1_pd(1.0);
	r[PAIRS + 1] = _mm256_set1_pd(1.0);
	r[PAIRS + 2] = _mm256_set1_pd(1.0);

	for (int i = 1, k = 0; i < NBODIES; i++) {
		for (int j = 0; j < i; j++, k++) {
			e -= masses[i] * masses[j] * w[k];
		}
	}

	energy_result = e;
})

BENCHMARK_FN(advance_simd, {
	r[PAIRS] = _mm256_set1_pd(1.0);
	r[PAIRS + 1] = _mm256_set1_pd(1.0);
	r[PAIRS + 2] = _mm256_set1_pd(1.0);

	__m256d rt = _mm256_set1_pd(0.01);

	__m256d rm[NBODIES];
	for (int i = 0; i < NBODIES; i++) {
		rm[i] = _mm256_set1_pd(masses[i]);
	}

	for (int s = 0; s < NB_ITERS; s++) {
		for (int k = 0; k < PAIRS; k += 4) {
			__m256d x = _mm256_load_pd(w + k);
			__m256d y = x * x;
			__m256d z = x * rt;
			x = y * z;
			_mm256_store_pd(w + k, x);
		}

		for (int i = 1, k = 0; i < NBODIES; i++) {
			for (int j = 0; j < i; j++, k++) {
				__m256d t = _mm256_set1_pd(w[k]);
				t = r[k] * t;
				__m256d x = t * rm[j];
				__m256d y = t * rm[i];

				velocities[i] = velocities[i] - x;
				velocities[j] = velocities[j] + y;
			}
		}

		for (int i = 0; i < NBODIES; i++) {
			__m256d t = velocities[i] * rt;
			positions[i] = positions[i] + t;
		}
	}
})

/**
 * @brief Run the benchmarks for the nbody problem
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss)
					  .RUN(initialize_bodies)
					  .RUN(offset_momentum)
					  .RUN(advance)
					  .RUN(energy)
					  .RUN(initialize_bodies_simd)
					  .RUN(rsqrt_simd)
					  .RUN(kernel_simd)
					  .RUN(energy_simd)
					  .RUN(advance_simd)
					  .results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}