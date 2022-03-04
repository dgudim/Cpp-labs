printf "compiling...\n"
g++ -x c++ ./test_cpp -o test_in
g++ -x c++ ./test_cpp -O3 -o test_in_o3

g++ -x c++ ./test2_cpp -o test_out
g++ -x c++ ./test2_cpp -O3 -o test_out_o3
printf "compiled\n\n"

time ./test_in;
time ./test_out;

printf "O3 optimization\n"
time ./test_in_o3;
time ./test_out_o3;
