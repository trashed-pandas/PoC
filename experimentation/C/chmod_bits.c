#include <stdio.h>
#include <sys/stat.h>


int main(){

	printf("%x\tS_ISUID\n", S_ISUID);
	printf("\n%x\tS_IRUSR\n", S_IRUSR);
	printf("%x\tS_IWUSR\n", S_IWUSR);
	printf("%x\tS_IXUSR\n", S_IXUSR);
	printf("\n%x\tS_IRGRP\n", S_IRGRP);
	printf("%x\tS_IWGRP\n", S_IWGRP);
	printf("%x\tS_IXGRP\n", S_IXGRP);
	printf("\n%x\tS_IROTH\n", S_IROTH);
	printf("%x\tS_IWOTH\n", S_IWOTH);
	printf("%x\tS_IXOTH\n", S_IXOTH);
	printf("\n%x\tS_ISUID|S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH\n", S_ISUID|S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);

	return(0);
}
