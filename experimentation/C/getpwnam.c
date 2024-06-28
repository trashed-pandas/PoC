
#include <errno.h>
#include <pwd.h>
#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char **argv){

	struct spwd *sp_ptr;
	struct passwd *pw_ptr;
	
	int i;
	long uid;
	char *endptr;

	for(i = 1; i < argc; i++){

		errno = 0;
		endptr = NULL;
		uid = strtol(argv[i], &endptr, 0);

		if(errno){
			fprintf(stderr, "strtol(): %s: %s\n", strerror(errno), argv[i]);
			continue;
		}

		printf("UID: %ld\n", uid);

		errno = 0;
		if(!(pw_ptr = getpwuid(uid))){
			if(errno){
				fprintf(stderr, "getpwuid(): %s: %ld\n", strerror(errno), uid);
			}else{
				printf("UID not found in passwd: %ld\n", uid);
			}
			continue;
		}
		printf("\tpw_uid: %d\n", pw_ptr->pw_uid);
		printf("\tpw_gid: %d\n", pw_ptr->pw_gid);
		printf("\tpw_gecos: %s\n", pw_ptr->pw_gecos);
		printf("\tpw_dir: %s\n", pw_ptr->pw_dir);
		printf("\tpw_shell: %s\n", pw_ptr->pw_shell);
		printf("\tpw_name: %s\n", pw_ptr->pw_name);
		printf("\tpw_passwd: %s\n", pw_ptr->pw_passwd);
		printf("\n");

		errno = 0;
		if(!(sp_ptr = getspnam(pw_ptr->pw_name))){
			if(errno){
				fprintf(stderr, "getspnam(): %s: %s\n", strerror(errno), pw_ptr->pw_name);
			}else{
				printf("User not found in shadow: %s\n", pw_ptr->pw_name);
			}
			continue;
		}
		printf("\tsp_uid: %s\n", sp_ptr->sp_namp);
		printf("\tsp_pwdp: %s\n", sp_ptr->sp_pwdp);
		printf("\tsp_lstchg: %ld\n", sp_ptr->sp_lstchg);
		printf("\tsp_min: %ld\n", sp_ptr->sp_min);
		printf("\tsp_max: %ld\n", sp_ptr->sp_max);
		printf("\tsp_warn: %ld\n", sp_ptr->sp_warn);
		printf("\tsp_inact: %ld\n", sp_ptr->sp_inact);
		printf("\tsp_expire: %ld\n", sp_ptr->sp_expire);
		printf("\tsp_flag: %ld\n", sp_ptr->sp_flag);
		printf("\n");

	}

	return 0;
}
