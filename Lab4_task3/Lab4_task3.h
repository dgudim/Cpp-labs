typedef double (*FunctionPointer)(double);

double cosXcosX(double x);
double cosXcosX_sum(double x);

double expXcosX(double x);
double expXcosX_sum(double x);

double expXexpX(double x);
double expXexpX_sum(double x);

double lerp(double from, double to, double progress);

void printGraph(FunctionPointer func, double from, double to, double step);
void printTable(FunctionPointer func, FunctionPointer func_sum, double from, double to, double step, bool print_sum, bool print_abs, bool print_graph);