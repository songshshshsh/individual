#include <cstdlib>
#include <cstdio>

using namespace std;

int main()
{
	for (int i = 8;i <= 10; ++i)
		for (int j = 2;j <= 4;++j)
			for (int k = 2;k <= 4;++k)
			{
				char command[233];
				sprintf(command, "mkdir %d_%d_%d", i, j, k);
				system(command);
			}
	for (int i = 8;i <= 10; ++i)
		for (int j = 2;j <= 4;++j)
			for (int k = 2;k <= 4;++k)
				for (int u = 1;u <= 10;++u)
				{
					FILE *tmp = fopen("tmp", "w");
					fprintf(tmp, "%d %d %d", i, j, k);
					fclose(tmp);
					char command[233];
					sprintf(command, "./main < tmp >> %d_%d_%d/out", i, j, k);
					printf("%s\n", command);
					system(command);
				}
	return 0;
}