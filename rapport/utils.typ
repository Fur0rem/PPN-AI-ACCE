#let bench_show(dir) = {
	// Set the page layout to have two main columns
	set align(center)
	grid(
		columns: (1fr, 1fr),
		row-gutter: 10pt,
		[
			// Align the images and text to the center
			#set align(center)

			// Prevent page breaks within the content
			// set page(break-before: "avoid")

			// First column content
			Benchs Unitaires
			#let rust_benchs = (
				"malloc",
				"calloc",
				"realloc",
			)
			#for bench in rust_benchs {
				image(dir + "/" + bench + ".svg", fit: "contain")
			}
		],
		[	
			#set align(center)
			Benchs de programmes
			#let c_benchs = (
				"big_allocs",
				"mixed_allocs",
				"small_allocs",
				"zeroed_matrix_10x100000",
				"zeroed_matrix_100000x10",
				"double_linked_list",
				"count_occurences",
				"dijkstra",
			)
			// Second column content with two sub-columns
			#grid(
				columns: (1fr, 1fr),
				row-gutter: 10pt,
				// First sub-column content
				[
					Temps d'exécution
					#for bench in c_benchs {
						let time = dir + "/" + bench + "_time.svg"
						image(time, width: 100%, height: 8%, fit: "contain")
					}
				],
				// Second sub-column content
				[
					Mémoire utilisée
					#for bench in c_benchs {
						let memory = dir + "/" + bench + "_memory.svg"
						image(memory, width: 100%, height: 8%, fit: "contain")
					}
				]
			)
		]
	)
}

#let code_snippet(code: none, title: none, tabwidth: 4, lang: "c") = {
	let spacing = " "
	let x = 1
	let replaced_code = ""
	while x < tabwidth {
		spacing = spacing + " "
		x = x + 1
	}
	for char in code {
		if char == "\t" {
			replaced_code = replaced_code + spacing
		} else {
			replaced_code = replaced_code + char
		}
	}
	set align(left)
	text(14pt)[
		#title
	]
	set align(left)
	text(10pt)[
		#raw(replaced_code, lang: lang)
	]
}

#import "@preview/gviz:0.1.0": *

#let draw_neural_network2(topology: list) = {
show raw.where(lang: "dot-render"): it => render-image(it.text)

```dot-render
digraph mygraph {
	rankdir=LR;
  A -> C;
  A -> D;
  A -> E;
  B -> C;
  B -> D;
  B -> E;
  C -> F;
  D -> F;
  E -> F;
}```
}


#let draw_neural_network(topology: list) = {
// With content.
circle[
  #set align(center + horizon)
]

}