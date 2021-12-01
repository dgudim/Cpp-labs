typedef double (*FunctionPointer)(double);
typedef double (*SumFunctionPointer)(double, int);

double cosXcosX(double x);
double cosXcosX_sum(double x, int n);

double expXcosX(double x);
double expXcosX_sum(double x, int n);

double expXexpX(double x);
double expXexpX_sum(double x, int n);

double lerp(double from, double to, double progress);

void printGraph(FunctionPointer func, double from, double to, double step);
void printTable(FunctionPointer func, SumFunctionPointer func_sum, double from, double to, double step, int n, bool print_sum, bool print_abs, bool print_graph);