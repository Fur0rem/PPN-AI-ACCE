var searchData=
[
  ['back_5fpropagate_0',['back_propagate',['../classLayer.html#afadf0f02441ae751fe4d77711d45aec3',1,'Layer']]],
  ['benchmark_5ffn_1',['benchmark_fn',['../codelet__livermore__default__lloops__c__endloop__line1795__3_8cpp.html#a063bbe44b163fedd02b50d36d93c6511',1,'BENCHMARK_FN(lloops_c_endloop_line1795_loop_c_3_c, { int __k_0__=k;int __i_1__=i;{ for(__k_0__=0;((long) __k_0__)&lt; inner_loops;__k_0__++) { int _lu_fringe_3=8;for(__i_1__=0;__i_1__&lt;=24 - _lu_fringe_3;__i_1__+=8) { m=((long)(25 *__k_0__+__i_1__+1));sumcheck[section][21]=sumcheck[section][21]+px[__k_0__][__i_1__] *((double) m);{ m=((long)(25 *__k_0__+(__i_1__+1)+1));sumcheck[section][21]=sumcheck[section][21]+px[__k_0__][__i_1__+1] *((double) m);} { m=((long)(25 *__k_0__+(__i_1__+2)+1));sumcheck[section][21]=sumcheck[section][21]+px[__k_0__][__i_1__+2] *((double) m);} { m=((long)(25 *__k_0__+(__i_1__+3)+1));sumcheck[section][21]=sumcheck[section][21]+px[__k_0__][__i_1__+3] *((double) m);} { m=((long)(25 *__k_0__+(__i_1__+4)+1));sumcheck[section][21]=sumcheck[section][21]+px[__k_0__][__i_1__+4] *((double) m);} { m=((long)(25 *__k_0__+(__i_1__+5)+1));sumcheck[section][21]=sumcheck[section][21]+px[__k_0__][__i_1__+5] *((double) m);} { m=((long)(25 *__k_0__+(__i_1__+6)+1));sumcheck[section][21]=sumcheck[section][21]+px[__k_0__][__i_1__+6] *((double) m);} { m=((long)(25 *__k_0__+(__i_1__+7)+1));sumcheck[section][21]=sumcheck[section][21]+px[__k_0__][__i_1__+7] *((double) m);} } for(;__i_1__&lt;=24;__i_1__+=1) { m=((long)(25 *__k_0__+__i_1__+1));sumcheck[section][21]=sumcheck[section][21]+px[__k_0__][__i_1__] *((double) m);} } } k=__k_0__;i=__i_1__;}) int main():&#160;codelet_livermore_default_lloops_c_endloop_line1795_3.cpp'],['../codelet__livermore__default__lloops__c__endloop__line1795__0_8cpp.html#a289fa288b4ab37c0dfd4e9781a8ac713',1,'BENCHMARK_FN(lloops_c_endloop_line1795_loop_c_0_c, { int __k_0__=k;int __i_1__=i;{ for(__k_0__=0;__k_0__&lt; inner_loops;__k_0__++) { for(__i_1__=0;__i_1__&lt; 25;__i_1__++) { m=(25 *__k_0__+__i_1__+1);sumcheck[section][21]=sumcheck[section][21]+px[__k_0__][__i_1__] *((double) m);} } } k=__k_0__;i=__i_1__;}) int main():&#160;codelet_livermore_default_lloops_c_endloop_line1795_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line1031__0_8cpp.html#a03fbf35d1b9d03e0e636d5b5c830ab11',1,'BENCHMARK_FN(lloops_c_kernels_line1031_loop_c_0_c, { int __k_0__=k;{ for(__k_0__=1;__k_0__&lt; inner_loops;__k_0__++) { x[__k_0__]=x[__k_0__ - 1]+y[__k_0__];} } k=__k_0__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line1031_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line1113__0_8cpp.html#a0cc13deda6dd1bdb848ba2184de85c87',1,'BENCHMARK_FN(lloops_c_kernels_line1113_loop_c_0_c, { int __k_0__=k;{ for(__k_0__=0;__k_0__&lt; inner_loops;__k_0__++) { vx[__k_0__]=vx[__k_0__]+ex1[__k_0__]+(xx[__k_0__] - xi[__k_0__]) *dex1[__k_0__];xx[__k_0__]=xx[__k_0__]+vx[__k_0__]+xtra[27];ir[__k_0__]=((long) xx[__k_0__]);rx[__k_0__]=xx[__k_0__] - ir[__k_0__];ir[__k_0__]=(ir[__k_0__] &amp;(2048 - 1))+1;xx[__k_0__]=rx[__k_0__]+ir[__k_0__];} } k=__k_0__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line1113_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line1113__3_8cpp.html#aec793d6bb049bf5f76c66b4f857e4224',1,'BENCHMARK_FN(lloops_c_kernels_line1113_loop_c_3_c, { int k0=k;{ int _lu_fringe_3=(((inner_loops - 1+1 - 0) % 1==0 ?(inner_loops - 1+1 - 0)/1 :(inner_loops - 1+1 - 0)/1+1)) % 8==0 ? 0 :8;for(k0=0;((long) k0)&lt;=inner_loops - 1 - _lu_fringe_3;k0+=8) { for(int x=0;x&lt; 8;x++) { vx[k0+x]=vx[k0+x]+ex1[k0+x]+(xx[k0+x] - xi[k0+x]) *dex1[k0+x];xx[k0+x]=xx[k0+x]+vx[k0+x]+xtra[27];ir[k0+x]=((long) xx[k0+x]);rx[k0+x]=xx[k0+x] -((double) ir[k0+x]);ir[k0+x]=(ir[k0+x] &amp;((long) 2047))+((long) 1);xx[k0+x]=rx[k0+x]+((double) ir[k0+x]);} } for(;((long) k0)&lt;=inner_loops - 1;k0+=1) { vx[k0]=vx[k0]+ex1[k0]+(xx[k0] - xi[k0]) *dex1[k0];xx[k0]=xx[k0]+vx[k0]+xtra[27];ir[k0]=((long) xx[k0]);rx[k0]=xx[k0] -((double) ir[k0]);ir[k0]=(ir[k0] &amp;((long) 2047))+((long) 1);xx[k0]=rx[k0]+((double) ir[k0]);} } k=k0;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line1113_3.cpp'],['../codelet__livermore__default__lloops__c__kernels__line1340__0_8cpp.html#af722cc8249777a71569f41bb406af497',1,'BENCHMARK_FN(lloops_c_kernels_line1340_loop_c_0_c, { int __k_0__=k;int __j_1__=j;{ for(__k_0__=1;__k_0__&lt; kn;__k_0__++) { for(__j_1__=1;__j_1__&lt; jnn;__j_1__++) { za[__k_0__][__j_1__]=(zp[__k_0__+1][__j_1__ - 1]+zq[__k_0__+1][__j_1__ - 1] - zp[__k_0__][__j_1__ - 1] - zq[__k_0__][__j_1__ - 1]) *(zr[__k_0__][__j_1__]+zr[__k_0__][__j_1__ - 1])/(zm[__k_0__][__j_1__ - 1]+zm[__k_0__+1][__j_1__ - 1]);zb[__k_0__][__j_1__]=(zp[__k_0__][__j_1__ - 1]+zq[__k_0__][__j_1__ - 1] - zp[__k_0__][__j_1__] - zq[__k_0__][__j_1__]) *(zr[__k_0__][__j_1__]+zr[__k_0__ - 1][__j_1__])/(zm[__k_0__][__j_1__]+zm[__k_0__][__j_1__ - 1]);} } } k=__k_0__;j=__j_1__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line1340_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line1351__0_8cpp.html#a58713af5754a6c03bab95a9f6f4ecb22',1,'BENCHMARK_FN(lloops_c_kernels_line1351_loop_c_0_c, { int __k_0__=k;int __j_1__=j;{ for(__k_0__=1;__k_0__&lt; kn;__k_0__++) { for(__j_1__=1;__j_1__&lt; jnn;__j_1__++) { zu[__k_0__][__j_1__]+=xtra[32] *(za[__k_0__][__j_1__] *(zz[__k_0__][__j_1__] - zz[__k_0__][__j_1__+1]) - za[__k_0__][__j_1__ - 1] *(zz[__k_0__][__j_1__] - zz[__k_0__][__j_1__ - 1]) - zb[__k_0__][__j_1__] *(zz[__k_0__][__j_1__] - zz[__k_0__ - 1][__j_1__])+zb[__k_0__+1][__j_1__] *(zz[__k_0__][__j_1__] - zz[__k_0__+1][__j_1__]));zv[__k_0__][__j_1__]+=xtra[32] *(za[__k_0__][__j_1__] *(zr[__k_0__][__j_1__] - zr[__k_0__][__j_1__+1]) - za[__k_0__][__j_1__ - 1] *(zr[__k_0__][__j_1__] - zr[__k_0__][__j_1__ - 1]) - zb[__k_0__][__j_1__] *(zr[__k_0__][__j_1__] - zr[__k_0__ - 1][__j_1__])+zb[__k_0__+1][__j_1__] *(zr[__k_0__][__j_1__] - zr[__k_0__+1][__j_1__]));} } } k=__k_0__;j=__j_1__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line1351_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line1351__64_8cpp.html#a267a6cc9cd0e84d9c3f8fb9bee3cae37',1,'BENCHMARK_FN(lloops_c_kernels_line1351_loop_c_64_c, { int k0=k;int j_1=j;{ for(j_1=1;((long) j_1)&lt; jnn;j_1++) { for(k0=1;((long) k0)&lt; kn;k0++) { zu[k0][j_1]+=xtra[32] *(za[k0][j_1] *(zz[k0][j_1] - zz[k0][j_1+1]) - za[k0][j_1 - 1] *(zz[k0][j_1] - zz[k0][j_1 - 1]) - zb[k0][j_1] *(zz[k0][j_1] - zz[k0 - 1][j_1])+zb[k0+1][j_1] *(zz[k0][j_1] - zz[k0+1][j_1]));zv[k0][j_1]+=xtra[32] *(za[k0][j_1] *(zr[k0][j_1] - zr[k0][j_1+1]) - za[k0][j_1 - 1] *(zr[k0][j_1] - zr[k0][j_1 - 1]) - zb[k0][j_1] *(zr[k0][j_1] - zr[k0 - 1][j_1])+zb[k0+1][j_1] *(zr[k0][j_1] - zr[k0+1][j_1]));} } } k=k0;j=j_1;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line1351_64.cpp'],['../codelet__livermore__default__lloops__c__kernels__line1444__0_8cpp.html#a9584c54ffce0d804d6f1cf390ccb60d4',1,'BENCHMARK_FN(lloops_c_kernels_line1444_loop_c_0_c, { int __k_0__=k;int __i_1__=i;int __j_2__=j;{ for(__k_0__=0;__k_0__&lt; 25;__k_0__++) { for(__i_1__=0;__i_1__&lt; 25;__i_1__++) { for(__j_2__=0;__j_2__&lt; inner_loops;__j_2__++) { px[__j_2__][__i_1__]+=vy[__k_0__][__i_1__] *cx[__j_2__][__k_0__];} } } } k=__k_0__;i=__i_1__;j=__j_2__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line1444_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line1444__144_8cpp.html#a42cb9f6080dca368851c76eac66d211b',1,'BENCHMARK_FN(lloops_c_kernels_line1444_loop_c_144_c, { int __k_0__=k;int __i_1__=i;int __j_2__=j;{ for(__i_1__=0;__i_1__&lt; 25;__i_1__++) { for(__j_2__=0;((long) __j_2__)&lt; inner_loops;__j_2__++) { for(__k_0__=0;__k_0__&lt; 25;__k_0__++) { px[__j_2__][__i_1__]+=vy[__k_0__][__i_1__] *cx[__j_2__][__k_0__];} } } } k=__k_0__;i=__i_1__;j=__j_2__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line1444_144.cpp'],['../codelet__livermore__default__lloops__c__kernels__line1444__156_8cpp.html#a60ab9e8b8de29d03ebd68b52968fac8f',1,'BENCHMARK_FN(lloops_c_kernels_line1444_loop_c_156_c, { int __k_0__=k;int __i_1__=i;int __j_2__=j;{ for(__i_1__=0;__i_1__&lt; 25;__i_1__++) { int _lu_fringe_222=(((inner_loops - 1+1 - 0) % 1==0 ?(inner_loops - 1+1 - 0)/1 :(inner_loops - 1+1 - 0)/1+1)) % 2==0 ? 0 :2;for(__j_2__=0;((long) __j_2__)&lt;=inner_loops - 1 - _lu_fringe_222;__j_2__+=2) { for(__k_0__=0;__k_0__&lt; 25;__k_0__++) { px[__j_2__][__i_1__]+=vy[__k_0__][__i_1__] *cx[__j_2__][__k_0__];px[__j_2__+1][__i_1__]+=vy[__k_0__][__i_1__] *cx[__j_2__+1][__k_0__];} } for(;((long) __j_2__)&lt;=inner_loops - 1;__j_2__+=1) { for(__k_0__=0;__k_0__&lt; 25;__k_0__++) { px[__j_2__][__i_1__]+=vy[__k_0__][__i_1__] *cx[__j_2__][__k_0__];} } } } k=__k_0__;i=__i_1__;j=__j_2__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line1444_156.cpp'],['../codelet__livermore__default__lloops__c__kernels__line771__0_8cpp.html#a59ed6a32a21c73b90db865b9027dfcab',1,'BENCHMARK_FN(lloops_c_kernels_line771_loop_c_0_c, { int __k_0__=k;{ for(__k_0__=0;__k_0__&lt; inner_loops;__k_0__++) { x[__k_0__]=xtra[28]+y[__k_0__] *(xtra[30] *z[__k_0__+10]+xtra[36] *z[__k_0__+11]);} } k=__k_0__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line771_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line869__0_8cpp.html#ace242a9701f0bc7c0e46cb1bcd8ff2f4',1,'BENCHMARK_FN(lloops_c_kernels_line869_loop_c_0_c, { int __i_0__=i;{ for(__i_0__=1;__i_0__&lt; inner_loops;__i_0__++) { x[__i_0__]=z[__i_0__] *(y[__i_0__] - x[__i_0__ - 1]);} } i=__i_0__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line869_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line913__0_8cpp.html#a99792873faf8868c7bc046cffe2ef063',1,'BENCHMARK_FN(lloops_c_kernels_line913_loop_c_0_c, { int __k_0__=k;{ for(__k_0__=0;__k_0__&lt; inner_loops;__k_0__++) { x[__k_0__]=u[__k_0__]+xtra[30] *(z[__k_0__]+xtra[30] *y[__k_0__])+xtra[36] *(u[__k_0__+3]+xtra[30] *(u[__k_0__+2]+xtra[30] *u[__k_0__+1])+xtra[36] *(u[__k_0__+6]+xtra[28] *(u[__k_0__+5]+xtra[28] *u[__k_0__+4])));} } k=__k_0__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line913_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line937__0_8cpp.html#a603175875c8ee497de55672a085e1730',1,'BENCHMARK_FN(lloops_c_kernels_line937_loop_c_0_c, { int __kx_0__=kx;int __ky_1__=ky;{ for(__kx_0__=1;__kx_0__&lt; 3;__kx_0__++) { for(__ky_1__=1;__ky_1__&lt; inner_loops;__ky_1__++) { du1[__ky_1__]=u1[nl1][__ky_1__+1][__kx_0__] - u1[nl1][__ky_1__ - 1][__kx_0__];du2[__ky_1__]=u2[nl1][__ky_1__+1][__kx_0__] - u2[nl1][__ky_1__ - 1][__kx_0__];du3[__ky_1__]=u3[nl1][__ky_1__+1][__kx_0__] - u3[nl1][__ky_1__ - 1][__kx_0__];u1[nl2][__ky_1__][__kx_0__]=u1[nl1][__ky_1__][__kx_0__]+xtra[1] *du1[__ky_1__]+xtra[2] *du2[__ky_1__]+xtra[3] *du3[__ky_1__]+xtra[34] *(u1[nl1][__ky_1__][__kx_0__+1] - 2.0 *u1[nl1][__ky_1__][__kx_0__]+u1[nl1][__ky_1__][__kx_0__ - 1]);u2[nl2][__ky_1__][__kx_0__]=u2[nl1][__ky_1__][__kx_0__]+xtra[4] *du1[__ky_1__]+xtra[5] *du2[__ky_1__]+xtra[6] *du3[__ky_1__]+xtra[34] *(u2[nl1][__ky_1__][__kx_0__+1] - 2.0 *u2[nl1][__ky_1__][__kx_0__]+u2[nl1][__ky_1__][__kx_0__ - 1]);u3[nl2][__ky_1__][__kx_0__]=u3[nl1][__ky_1__][__kx_0__]+xtra[7] *du1[__ky_1__]+xtra[8] *du2[__ky_1__]+xtra[9] *du3[__ky_1__]+xtra[34] *(u3[nl1][__ky_1__][__kx_0__+1] - 2.0 *u3[nl1][__ky_1__][__kx_0__]+u3[nl1][__ky_1__][__kx_0__ - 1]);} } } kx=__kx_0__;ky=__ky_1__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line937_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line971__0_8cpp.html#a04daa41fb667fef4a866ddac8ecde93a',1,'BENCHMARK_FN(lloops_c_kernels_line971_loop_c_0_c, { int __i_0__=i;{ for(__i_0__=0;__i_0__&lt; inner_loops;__i_0__++) { px[__i_0__][0]=xtra[22] *px[__i_0__][12]+xtra[21] *px[__i_0__][11]+xtra[20] *px[__i_0__][10]+xtra[19] *px[__i_0__][9]+xtra[18] *px[__i_0__][8]+xtra[17] *px[__i_0__][7]+xtra[16] *px[__i_0__][6]+xtra[12] *(px[__i_0__][4]+px[__i_0__][5])+px[__i_0__][2];} } i=__i_0__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line971_0.cpp'],['../codelet__livermore__default__lloops__c__kernels__line993__0_8cpp.html#a782e76f4920385a69da4eeaf0b8bf2c4',1,'BENCHMARK_FN(lloops_c_kernels_line993_loop_c_0_c, { int __i_0__=i;{ for(__i_0__=0;__i_0__&lt; inner_loops;__i_0__++) { ar=cx[__i_0__][4];br=ar - px[__i_0__][4];px[__i_0__][4]=ar;cr=br - px[__i_0__][5];px[__i_0__][5]=br;ar=cr - px[__i_0__][6];px[__i_0__][6]=cr;br=ar - px[__i_0__][7];px[__i_0__][7]=ar;cr=br - px[__i_0__][8];px[__i_0__][8]=br;ar=cr - px[__i_0__][9];px[__i_0__][9]=cr;br=ar - px[__i_0__][10];px[__i_0__][10]=ar;cr=br - px[__i_0__][11];px[__i_0__][11]=br;px[__i_0__][13]=cr - px[__i_0__][12];px[__i_0__][12]=cr;} } i=__i_0__;}) int main():&#160;codelet_livermore_default_lloops_c_kernels_line993_0.cpp'],['../codelet__nn__backpropagate__second__dervatives__line262_8cpp.html#af8d69f4b9b96d56c157ac12c57dab87c',1,'BENCHMARK_FN(codelet_nn_backpropagate_second_dervatives_line262, { for(int i=0;i&lt;=I_SIZE - 1;i+=1) { for(int j=0;j&lt;=MAX_COL - 1;j+=1) { differentials[i][j]=0.0;} } }) int main():&#160;codelet_nn_backpropagate_second_dervatives_line262.cpp'],['../codelet__rbm__train_8cpp.html#a37e00adfbdf4621c704c04ee397090a7',1,'BENCHMARK_FN(codelet_rbm_train_line191, { for(int v=0;v&lt;=500;v+=1) { for(int h=0;h&lt;=300;h+=1) { edges[v][h]=edges[v][h]+0.1 *(pos[v][h] - neg[v][h]);} } }) int main():&#160;codelet_rbm_train.cpp'],['../codelet__srr__engine_8cpp.html#a8bb4764ac16c143f825f85d2a61b9a08',1,'BENCHMARK_FN(codelet_srr_engine_matmul_line21_1, { for(int i=0;i&lt; N;i++) { for(int j=0;j&lt; N;j++) { m_result[i]+=mat1[i][j] *mat2[j];} } }) BENCHMARK_FN(codelet_srr_engine_matmul_line21_8:&#160;codelet_srr_engine.cpp'],['../codelet__vector__c__hypre__seq__vector__axpy__line384_8cpp.html#a679965e7e7e1be5281449ecae6060783',1,'BENCHMARK_FN(codelet_vector_c_hypre_seq_vector_axpy_line384, { for(int i=0;i&lt;=SIZE - 1;i+=1) { y_data[i]+=alpha *x_data[i];} }) int main():&#160;codelet_vector_c_hypre_seq_vector_axpy_line384.cpp'],['../fibonacci_8cpp.html#a6b545ae3fce2c22a77cd74e70276ae2f',1,'BENCHMARK_FN(fib_iter, { int a=0;int b=1;int c;for(int i=2;i&lt;=N;i++) { c=a+b;a=b;b=c;} fib_result=b;}) BENCHMARK_FN(fib_float_slow_exp:&#160;fibonacci.cpp'],['../fibonacci_8cpp.html#ad0ba1e00f5fb7ab6000486d9f34c3e25',1,'BENCHMARK_FN(fib_float_fast_exp, { float phi=(1+sqrt(5))/2;float psi=(1 - sqrt(5))/2;float phi_N=pow(phi, N);float psi_N=pow(psi, N);float fib=(phi_N - psi_N)/sqrt(5);fib_float_result=fib;}) BENCHMARK_FN(fib_recursive:&#160;fibonacci.cpp'],['../nbody_8cpp.html#a387f6e7aab3ff5ea40077b0103d77bad',1,'BENCHMARK_FN(initialize_bodies, {({ bodies[0]=Planet{ .x=0,.y=0,.z=0,.vx=0,.vy=0,.vz=0,.mass=SOLAR_MASS, };bodies[1]=Planet{.x=4.84143144246472090e+00,.y=-1.16032004402742839e+00,.z=-1.03622044471123109e-01,.vx=1.66007664274403694e-03 *DAYS_PER_YEAR,.vy=7.69901118419740425e-03 *DAYS_PER_YEAR,.vz=-6.90460016972063023e-05 *DAYS_PER_YEAR,.mass=9.54791938424326609e-04 *SOLAR_MASS};bodies[2]=Planet{.x=8.34336671824457987e+00,.y=4.12479856412430479e+00,.z=-4.03523417114321381e-01,.vx=-2.76742510726862411e-03 *DAYS_PER_YEAR,.vy=4.99852801234917238e-03 *DAYS_PER_YEAR,.vz=2.30417297573763929e-05 *DAYS_PER_YEAR,.mass=2.85885980666130812e-04 *SOLAR_MASS};bodies[3]=Planet{.x=1.28943695621391310e+01,.y=-1.51111514016986312e+01,.z=-2.23307578892655734e-01,.vx=2.96460137564761618e-03 *DAYS_PER_YEAR,.vy=2.37847173959480950e-03 *DAYS_PER_YEAR,.vz=-2.96589568540237556e-05 *DAYS_PER_YEAR,.mass=4.36624404335156298e-05 *SOLAR_MASS};bodies[4]=Planet{.x=1.53796971148509165e+01,.y=-2.59193146099879641e+01,.z=1.79258772950371181e-01,.vx=2.68067772490389322e-03 *DAYS_PER_YEAR,.vy=1.62824170038242295e-03 *DAYS_PER_YEAR,.vz=-9.51592254519715870e-05 *DAYS_PER_YEAR,.mass=5.15138902046611451e-05 *SOLAR_MASS};});}) BENCHMARK_FN(offset_momentum:&#160;nbody.cpp'],['../nbody_8cpp.html#af7415a18ce0dae8ad8b6b9e95af78dcf',1,'BENCHMARK_FN(advance_simd, { r[PAIRS]=_mm256_set1_pd(1.0);r[PAIRS+1]=_mm256_set1_pd(1.0);r[PAIRS+2]=_mm256_set1_pd(1.0);__m256d rt=_mm256_set1_pd(0.01);__m256d rm[NBODIES];for(int i=0;i&lt; NBODIES;i++) { rm[i]=_mm256_set1_pd(masses[i]);} for(int s=0;s&lt; NB_ITERS;s++) { for(int k=0;k&lt; PAIRS;k+=4) { __m256d x=_mm256_load_pd(w+k);__m256d y=x *x;__m256d z=x *rt;x=y *z;_mm256_store_pd(w+k, x);} for(int i=1, k=0;i&lt; NBODIES;i++) { for(int j=0;j&lt; i;j++, k++) { __m256d t=_mm256_set1_pd(w[k]);t=r[k] *t;__m256d x=t *rm[j];__m256d y=t *rm[i];velocities[i]=velocities[i] - x;velocities[j]=velocities[j]+y;} } for(int i=0;i&lt; NBODIES;i++) { __m256d t=velocities[i] *rt;positions[i]=positions[i]+t;} } }) int main():&#160;nbody.cpp'],['../reverse__complement_8cpp.html#a182c833a75a5d3ed0589bb0f033fbb9a',1,'BENCHMARK_FN(reverse_complement, { char from[61];char to[61];for(int i=0;i&lt; 128;i++) { tbl[i]=i;} for(int i=0;i&lt; 60;i++) { from[i]=&quot;ATCG&quot;[i % 4];} from[60]=&apos;\0&apos;;size_t len=to - from;size_t off=60 -(len % 61);if(off) { char *m;for(m=from+60 - off;m&lt; to;m+=61) { memmove(m+1, m, off); *m=&apos;\n&apos;;} } for(int i=0;i&lt; 60;i++) { to[i]=tbl[from[i]];} to[60]=&apos;\0&apos;;}) int main():&#160;reverse_complement.cpp'],['../codelet__generate__seq__laplacian__line73_8cpp.html#af02777f0aca7184768e2daa742bf785c',1,'BENCHMARK_FN(codelet_generate_seq_laplacian_line73, { for(int i=0;i&lt;=GRID_SIZE - 1;i+=1) { x_data[i]=0.0;sol_data[i]=0.0;rhs_data[i]=1.0;} }) int main():&#160;codelet_generate_seq_laplacian_line73.cpp'],['../array__add_8cpp.html#a609dbb65b8c734642c7fcf9081eb9306',1,'BENCHMARK_FN(array_add_f32_no_simd, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;movss (%0, %%rcx, 4), %%xmm0\n&quot; &quot;movss (%1, %%rcx, 4), %%xmm1\n&quot; &quot;addss %%xmm1, %%xmm0\n&quot; &quot;movss %%xmm0, (%2, %%rcx, 4)\n&quot; &quot;addq $1, %%rcx\n&quot; &quot;cmpq %3, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(f32a), &quot;r&quot;(f32b), &quot;r&quot;(f32c), &quot;r&quot;((long long) N) :&quot;xmm0&quot;, &quot;xmm1&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(array_add_f32_simd128:&#160;array_add.cpp'],['../array__add_8cpp.html#a330d9c79e8c921819c14162e39731ac9',1,'BENCHMARK_FN(array_add_f32_simd256, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;vmovaps (%0, %%rcx, 4), %%ymm0\n&quot; &quot;vmovaps (%1, %%rcx, 4), %%ymm1\n&quot; &quot;vaddps %%ymm1, %%ymm0, %%ymm0\n&quot; &quot;vmovaps %%ymm0, (%2, %%rcx, 4)\n&quot; &quot;addq $8, %%rcx\n&quot; &quot;cmpq %3, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(f32a), &quot;r&quot;(f32b), &quot;r&quot;(f32c), &quot;r&quot;((long long) N) :&quot;ymm0&quot;, &quot;ymm1&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(array_add_f64_no_simd:&#160;array_add.cpp'],['../array__add_8cpp.html#a2a74fc29506354135c99afe080fa393b',1,'BENCHMARK_FN(array_add_f64_simd128, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;movapd (%0, %%rcx, 8), %%xmm0\n&quot; &quot;movapd (%1, %%rcx, 8), %%xmm1\n&quot; &quot;addpd %%xmm1, %%xmm0\n&quot; &quot;movapd %%xmm0, (%2, %%rcx, 8)\n&quot; &quot;addq $2, %%rcx\n&quot; &quot;cmpq %3, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(f64a), &quot;r&quot;(f64b), &quot;r&quot;(f64c), &quot;r&quot;((long long) N) :&quot;xmm0&quot;, &quot;xmm1&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(array_add_f64_simd256:&#160;array_add.cpp'],['../array__copy_8cpp.html#a25c5d8335f56cd6dee73a2aaabae8f72',1,'BENCHMARK_FN(u8_copy_no_simd, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;movb (%0, %%rcx, 1), %%al\n&quot; &quot;movb %%al, (%1, %%rcx, 1)\n&quot; &quot;addq $1, %%rcx\n&quot; &quot;cmpq %2, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(u8_src), &quot;r&quot;(u8_dst), &quot;r&quot;((long long) N) :&quot;al&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(u8_copy_simd128:&#160;array_copy.cpp'],['../array__copy_8cpp.html#a1b629d76418fceffad28bcf6d3d870e7',1,'BENCHMARK_FN(u8_copy_simd256, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;vmovdqu (%0, %%rcx, 1), %%ymm0\n&quot; &quot;vmovdqu %%ymm0, (%1, %%rcx, 1)\n&quot; &quot;addq $32, %%rcx\n&quot; &quot;cmpq %2, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(u8_src), &quot;r&quot;(u8_dst), &quot;r&quot;((long long) N) :&quot;ymm0&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(u16_copy_no_simd:&#160;array_copy.cpp'],['../array__copy_8cpp.html#af499f061592032004f56f68b3eef78c2',1,'BENCHMARK_FN(u16_copy_simd128, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;movdqu (%0, %%rcx, 2), %%xmm0\n&quot; &quot;movdqu %%xmm0, (%1, %%rcx, 2)\n&quot; &quot;addq $16, %%rcx\n&quot; &quot;cmpq %2, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(u16_src), &quot;r&quot;(u16_dst), &quot;r&quot;((long long) N) :&quot;xmm0&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(u16_copy_simd256:&#160;array_copy.cpp'],['../array__copy_8cpp.html#ab6cff7a2b37119b6d0c59163aefce634',1,'BENCHMARK_FN(u32_copy_no_simd, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;movl (%0, %%rcx, 4), %%eax\n&quot; &quot;movl %%eax, (%1, %%rcx, 4)\n&quot; &quot;addq $4, %%rcx\n&quot; &quot;cmpq %2, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(u32_src), &quot;r&quot;(u32_dst), &quot;r&quot;((long long) N) :&quot;eax&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(u32_copy_simd128:&#160;array_copy.cpp'],['../array__copy_8cpp.html#a57935093fc631de325f6bbb46c082f89',1,'BENCHMARK_FN(u32_copy_simd256, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;vmovdqa (%0, %%rcx, 4), %%ymm0\n&quot; &quot;vmovdqa %%ymm0, (%1, %%rcx, 4)\n&quot; &quot;addq $32, %%rcx\n&quot; &quot;cmpq %2, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(u32_src), &quot;r&quot;(u32_dst), &quot;r&quot;((long long) N) :&quot;ymm0&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(u64_copy_no_simd:&#160;array_copy.cpp'],['../array__copy_8cpp.html#a6d634dea13f72e38796edde59bf00638',1,'BENCHMARK_FN(u64_copy_simd128, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;movdqa (%0, %%rcx, 8), %%xmm0\n&quot; &quot;movdqa %%xmm0, (%1, %%rcx, 8)\n&quot; &quot;addq $16, %%rcx\n&quot; &quot;cmpq %2, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(u64_src), &quot;r&quot;(u64_dst), &quot;r&quot;((long long) N) :&quot;xmm0&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(u64_copy_simd256:&#160;array_copy.cpp'],['../array__copy_8cpp.html#a2aff0037b56ba7d37609867963f61238',1,'BENCHMARK_FN(f32_copy_no_simd, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;movss (%0, %%rcx, 4), %%xmm0\n&quot; &quot;movss %%xmm0, (%1, %%rcx, 4)\n&quot; &quot;addq $4, %%rcx\n&quot; &quot;cmpq %2, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(f32_src), &quot;r&quot;(f32_dst), &quot;r&quot;((long long) N) :&quot;xmm0&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(f32_copy_simd128:&#160;array_copy.cpp'],['../array__copy_8cpp.html#a29a2a5a713305dfc2d19ab698af594d8',1,'BENCHMARK_FN(f32_copy_simd256, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;vmovaps (%0, %%rcx, 4), %%ymm0\n&quot; &quot;vmovaps %%ymm0, (%1, %%rcx, 4)\n&quot; &quot;addq $32, %%rcx\n&quot; &quot;cmpq %2, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(f32_src), &quot;r&quot;(f32_dst), &quot;r&quot;((long long) N) :&quot;ymm0&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(f64_copy_no_simd:&#160;array_copy.cpp'],['../array__copy_8cpp.html#a1215775b1217479009680adf6309473d',1,'BENCHMARK_FN(f64_copy_simd128, { asm volatile(&quot;movq $0, %%rcx\n&quot; &quot;1:\n&quot; &quot;movapd (%0, %%rcx, 8), %%xmm0\n&quot; &quot;movapd %%xmm0, (%1, %%rcx, 8)\n&quot; &quot;addq $16, %%rcx\n&quot; &quot;cmpq %2, %%rcx\n&quot; &quot;jl 1b\n&quot; ::&quot;r&quot;(f64_src), &quot;r&quot;(f64_dst), &quot;r&quot;((long long) N) :&quot;xmm0&quot;, &quot;rcx&quot;);}) BENCHMARK_FN(f64_copy_simd256:&#160;array_copy.cpp'],['../array__copy_8cpp.html#a428ba7837ae89acd2fc4aa473a0db525',1,'BENCHMARK_FN(f64_copy_duffs_device, { volatile double *src=f64_src;volatile double *dst=f64_dst;int n=N;int count=(n+7)/8;switch(n % 8) { case 0:do { *dst++= *src++;case 7:*dst++= *src++;case 6:*dst++= *src++;case 5:*dst++= *src++;case 4:*dst++= *src++;case 3:*dst++= *src++;case 2:*dst++= *src++;case 1:*dst++= *src++;} while(--count &gt; 0);} }) int main():&#160;array_copy.cpp'],['../array__min_8cpp.html#aa8af7a3380ad37af108999a56334ab3a',1,'BENCHMARK_FN(u8_min_no_simd, { volatile uint8_t min=U8[0];for(int i=1;i&lt; N;i++) { if(U8[i]&lt; min) { min=U8[i];} } }) BENCHMARK_FN(u16_min_no_simd:&#160;array_min.cpp'],['../array__min_8cpp.html#acce6b07fb871cc2572cefa7976ceadd2',1,'BENCHMARK_FN(u32_min_no_simd, { volatile uint32_t min=U32[0];for(int i=1;i&lt; N;i++) { if(U32[i]&lt; min) { min=U32[i];} } }) BENCHMARK_FN(u64_min_no_simd:&#160;array_min.cpp'],['../array__min_8cpp.html#a08a43d027c1ced8871c9e73000e7447f',1,'BENCHMARK_FN(f32_min_no_simd, { volatile float min=F32[0];for(int i=1;i&lt; N;i++) { if(F32[i]&lt; min) { min=F32[i];} } }) BENCHMARK_FN(f64_min_no_simd:&#160;array_min.cpp'],['../array__min_8cpp.html#a20a50454e156fece303b360959b8df2c',1,'BENCHMARK_FN(u8_min_simd128, { volatile uint8_t min=U8[0];__m128i min_v=_mm_set1_epi8(min);for(int i=1;i&lt; N;i+=16) { __m128i v=_mm_load_si128((__m128i *)&amp;U8[i]);min_v=_mm_min_epu8(min_v, v);} min=_mm_extract_epi8(min_v, 0);}) BENCHMARK_FN(u8_min_simd256:&#160;array_min.cpp'],['../array__min_8cpp.html#ac30b921be3bceb29ca3ffbdb14c07d2c',1,'BENCHMARK_FN(u16_min_simd128, { volatile uint16_t min=U16[0];__m128i min_v=_mm_set1_epi16(min);for(int i=1;i&lt; N;i+=8) { __m128i v=_mm_load_si128((__m128i *)&amp;U16[i]);min_v=_mm_min_epu16(min_v, v);} min=_mm_extract_epi16(min_v, 0);}) BENCHMARK_FN(u16_min_simd256:&#160;array_min.cpp'],['../array__min_8cpp.html#a9e930f4e32fb1f710ea8e58cae5a9856',1,'BENCHMARK_FN(u32_min_simd128, { volatile uint32_t min=U32[0];__m128i min_v=_mm_set1_epi32(min);for(int i=1;i&lt; N;i+=4) { __m128i v=_mm_load_si128((__m128i *)&amp;U32[i]);min_v=_mm_min_epu32(min_v, v);} min=_mm_extract_epi32(min_v, 0);}) BENCHMARK_FN(u32_min_simd256:&#160;array_min.cpp'],['../array__min_8cpp.html#a6c7405c77e225757b0acd78ba0e32869',1,'BENCHMARK_FN(f32_min_simd128, { volatile float min=F32[0];__m128 min_v=_mm_set1_ps(min);for(int i=1;i&lt; N;i+=4) { __m128 v=_mm_load_ps(&amp;F32[i]);min_v=_mm_min_ps(min_v, v);} min=_mm_cvtss_f32(min_v);}) BENCHMARK_FN(f32_min_simd256:&#160;array_min.cpp'],['../array__min_8cpp.html#a00029d97d713f94184ffc182568b0be9',1,'BENCHMARK_FN(f64_min_simd128, { volatile double min=F64[0];__m128d min_v=_mm_set1_pd(min);for(int i=1;i&lt; N;i+=2) { __m128d v=_mm_load_pd(&amp;F64[i]);min_v=_mm_min_pd(min_v, v);} min=_mm_cvtsd_f64(min_v);}) BENCHMARK_FN(f64_min_simd256:&#160;array_min.cpp'],['../codelet__c__k__means__line109_8cpp.html#a16cb3df1f6624c70d81ffce3de031a57',1,'BENCHMARK_FN(codelet_c_k_means_line109, { for(int i=0;i&lt; K;i++) { for(int j=0;j&lt; M;j++) { c[i][j]=(counts[i] ? c1[i][j]/counts[i] :c1[i][j]);} } }) int main():&#160;codelet_c_k_means_line109.cpp'],['../codelet__crystal__cholesky_8cpp.html#a5aa3b8c2ae4ccf6e1fb147b1361505db',1,'BENCHMARK_FN(codelet_crystal_div_line61_loop, { for(int n=0;n&lt;=NB_SLIP - 1;n+=1) { bor_array[n]=1/dtcdgd[n][n];} }) int main():&#160;codelet_crystal_cholesky.cpp']]],
  ['binary_5ffrom_5fhexadecimal_2',['binary_from_hexadecimal',['../tests_2binary__with__split__parser_8cpp.html#acd878c1d88b0e5bbe0331ca11ff9a466',1,'binary_from_hexadecimal(std::vector&lt; uint8_t &gt; &amp;&amp;hex):&#160;binary_with_split_parser.cpp'],['../tests_2asm__parser_8cpp.html#acd878c1d88b0e5bbe0331ca11ff9a466',1,'binary_from_hexadecimal(std::vector&lt; uint8_t &gt; &amp;&amp;hex):&#160;asm_parser.cpp'],['../tests_2binary__parser_8cpp.html#acd878c1d88b0e5bbe0331ca11ff9a466',1,'binary_from_hexadecimal(std::vector&lt; uint8_t &gt; &amp;&amp;hex):&#160;binary_parser.cpp']]],
  ['binary_5fto_5fhexadecimal_3',['binary_to_hexadecimal',['../tests_2asm__parser_8cpp.html#a053f7be4bed0dc985cb3e571f62c05b8',1,'asm_parser.cpp']]],
  ['binaryparser_4',['BinaryParser',['../classBinaryParser.html#a5f007b25b06f99b7c074558e564c01de',1,'BinaryParser']]],
  ['binarywithsplitparser_5',['BinaryWithSplitParser',['../classBinaryWithSplitParser.html#a66b43a97c643dbf393bdda9469e7cd90',1,'BinaryWithSplitParser']]]
];
