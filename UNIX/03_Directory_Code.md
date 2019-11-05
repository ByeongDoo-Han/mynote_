## directory
### get current working directory
```c
int main(void){
	char ch[100]={0};

	getcwd(ch, 99);
	printf("%s\n", ch);	// /home/account/fac/tspark/CLASS/UNIX19/LAB3

	return 0;
}
```
### string tokenizing, find last directory name
```c
int main(void) {
	char ch[100]={0};
	char *token, *parr[10] = {NULL,};
	int i = 0;

	mkdir("T1", 0700);
	chdir("T1");
	mkdir("T2", 0700);
	chdir("T2");
	mkdir("T3", 0700);
	chdir("T3");

	getcwd(ch, 99);
	token = strtok(ch, "/");
	while(token != NULL) {
		parr[i] = token;
		i++;
		token = strtok(NULL, "/");
	}
	printf("%s\n", parr[i-1]);
	printf("%d", i);
	return 0;
}
```
### (opendir, readdir, chdir) command "cd", "ls" code
> lab5-1
```c
void change_directory(char *name){
	// cd 명령수행
	chdir(name);
	return;
}

void list() {
	// ls 명령수행
	DIR *dp;
	struct dirent *d;

	dp = opendir(".");
	d = readdir(dp);
	while(d!=NULL){
		printf("%s ", d->d_name);
		d = readdir(dp);
		// .과 ..을 제외하고 출력할 수 있는 if문
		//if (d->d_name[0]) != '.') {
		//	printf("%s", d->d_name);
		//}
	}
	printf("\n");
	return;
}

int main(void) {
	char name[50], cwd[100] = {0};
	
	while(1) {
		// 현재 working directory 이름 표시
		getcwd(cwd, 99);
		printf("%s>", cwd);

		scanf("%s", name);
		if(strcmp(name, "cd")==0){
			scanf("%s", name);
			change_directory(name);
		}
		else if(strcmp(name, "ls")==0){
			list();
		}
		else
			break;
	}

	return 0;
}
``` 
### cd, ls advance
> lab5-2
```c
void change_directory(char *cdname){
	chdir(cdname);
}
void list(){
	int i = 0;
	char *time;
	DIR *dp;
	struct dirent *d;
	struct stat buf;
	
	dp = opendir(".");
	d = readdir(dp);
	while(d != NULL){
		i = 0;
		stat(d->d_name, &buf);
		printf("%o ", buf.st_mode&0777);
		printf("%d ", buf.st_nlink);
		printf("%d ", buf.st_uid);
		printf("%d ", buf.st_gid);
		printf("%ld ", (long)buf.st_size);
		time = ctime(&buf.st_mtime);
		while(time[i] != '\n'){
			printf("%c", time[i]);
			i++;
		}
		printf(" %s ", d->d_name);
		if(S_ISREG(buf.st_mode))
			printf("F\n");
		else if(S_ISDIR(buf.st_mode))
			printf("D\n");
		d = readdir(dp);
	}		
}

int main(void) {
	char name[50], cwd[50];

	while(1){
		// show current working directory
		getcwd(cwd, sizeof(char)*49);
		printf("%s >", cwd);

		scanf("%s", name);
		if(!strcmp(name, "cd")){
			scanf("%s",name);
			change_directory(name);
		}
		else if(!strcmp(name, "ls")){
			list();
		}
		else
			break;
	}
	
	return 0;
}

```
### FTW
> lab6-1
```c
int list(const char *name, const struct stat *status, int type){
	if(type==FTW_NS)
		return 0;
	if(type==FTW_F){
		if(S_IXUSR&status->st_mode || S_IXGRP&status->st_mode || S_IXOTH&status->st_mode)
			printf("%-30s\t%3o\n", name, status->st_size);
	}
	else if(type==FTW_D || tpye==FTW_DNR)
		printf("%-30s*\t0%3o\n", name, status->st_size);
	return 0;
}

int main(void){
	ftw(".", list, 1);
	return 0;
}

```
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTY4NTYyNjE4N119
-->