#include <iostream>
#include <ctime>
#include "omp.h"
#define Max 10000


void init(int* A)
{
 for(long i=0; i<Max; i++ ){ A[i] = i; };
}

int*  add_two_vectors(const int* A ,const int* B)
{
	double start , end;
	int* C = new int[Max];
	start = omp_get_wtime();
	//#pragma omp parallel
	//#pragma omp for
	for(long i=0; i<Max; i++){
		C[i] = (A[i] + B[i])*1234;
	}
	end = omp_get_wtime();
 std::cout<<"start time :"<<start<<std::endl;
 std::cout<<"end time :"<<end<<std::endl;
 std::cout<<"the time taken is :"<<end - start<<std::endl;
 return C;
}
int reduction(int* p , int* q)
{
	int r = 0;
	double start ,end;
	start = omp_get_wtime();
	//#pragma omp simd reduction(+:r) aligned(p,q:32)
	for(int i=0; i<Max; i++){
		p[i]=q[i]*2;
		r += p[i];
	}
	end = omp_get_wtime();
 std::cout<<"start time :"<<start<<std::endl;
 std::cout<<"end time :"<<end<<std::endl;
 std::cout<<"the time taken is :"<<end - start<<std::endl;
	return r;

}

int main()
{
 int* A = new int[Max];
 int* B = new int[Max];
 init(A);
 init(B);
 //add_two_vectors(A,B);
 reduction(A,B);
 //# pragma omp parallel
 //{
 //	std::cout<<"Hello from thread"<<omp_get_thread_num()<<std::endl;

 //}
 std::cout<<"--------end---------------"<<std::endl;
 return 0;
}
