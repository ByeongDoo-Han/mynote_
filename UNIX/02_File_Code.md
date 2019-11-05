## file
### write "data1" read "data2" and print using (lseek SEEK_SET, SEEK_END)
> Lab 3-1
```c
int main(void){
	int i, fd, p, n, data1[10], data2[10];
	for (i=0;i<10;i++){
		scanf("%d", &data1[i]);
	}
	// 배열 data1[]에 저장 된 정수를 “data1" 파일에 저장한다.
	fd = open("data1", O_RDWR | O_CREAT, 0600);
	write(fd, data1, sizeof(int)*10);
	// “data1" 파일에 저장된 정수를 읽어 data2[] 배열에 저장한다.
	n = lseek(fd, 0, SEEK_END); 
	p = lseek(fd, 0, SEEK_SET);
	//lseek(fd, 0, SEEK_SET);
	
	//read(fd, data2, sizeof(int)*10);
	read(fd, data2, n-p);
	for (i=0;i<10;i++){
		printf("%-5d", data1[i]);
	}
	printf("\n");
        for (i=0;i<10;i++){
       		printf("%-5d", data2[i]);
        }
        printf("\n");
        
	return 0;
}
```
### lseek file pointer
> lab3-2
```c
int main() {
	int fd, i, n;
	char ch = 'X';
	// a
	fd = open("data2", O_RDWR | O_CREAT, 0600);

	for(i=0; i<10; i++){
		write(fd, &ch, 1);		// XXXXXXXXXX(*) 10개
	}							//(1)	     (2) 포인터 가리키고 있음
	// b
	lseek(fd, 0, SEEK_SET);		//(1)
	ch = 'Y';
	write(fd, &ch, 1);
	// c
	lseek(fd, 0, SEEK_END);		//(2)	// YXXXXXXXXXY	11개
	write(fd, &ch, 1);
	// d
	ch = 'Z';
	for(i=1; i<8; i+=2){
		lseek(fd, i, SEEK_SET);		// YZXZXZXZXXY
		write(fd, &ch, 1);
	}
	// e
	ch = 'T';
	lseek(fd, 14, SEEK_SET);		// YZXZXZXZXXY()()()T
	write(fd, &ch, 1);
	// f
	ch = 'S';
	lseek(fd, -2, SEEK_END);		// YZXZXZXZXXY()()ST
	write(fd, &ch, 1);
	// g
	ch = 'W';
	lseek(fd, -2, SEEK_CUR);		// YZXZXZXZXXY()WST
	write(fd, &ch, 1);
	
	return 0;
}
```
### input type INTEGER, write specific index using lseek
>lab3-3
```c
int main(void) {
	int i, fd, k , k1, data1[10], data2[10];

	fd = open("data3", O_RDWR | O_CREAT, 0600);

	for (i=0;i<10;i++){
		scanf("%d", &data1[i]);
	}

	// data1 배열의 0-4번째 정수를 "data3" 파일의 0, 2, 4, 6, 8번째 위치에 저장
	lseek(fd, 0, SEEK_CUR);
	for(i=0;i<5;i++){
		write(fd, &data1[i], sizeof(int));
		lseek(fd, sizeof(int), SEEK_CUR);
	}
	// data1 배열의 5-9번째 정수를 "data3" 파일의 1, 3, 5 ,7 ,9번째 위치에 저장
	lseek(fd, -9*sizeof(int), SEEK_CUR);
	for(i=5;i<10;i++){
		write(fd, &data1[i], sizeof(int));
		lseek(fd, sizeof(int), SEEK_CUR);
	}
	// "data3" 파일에 저장된 정수를 10개 읽어 data2 배열에 저장
	lseek(fd, 0, SEEK_SET);
	for(i=0;i<10;i++){
		read(fd, &data2[i], sizeof(int));
	}

	for (i=0;i<10;i++){
		printf("%-5d", data1[i]);
	}
	printf("\n");

	for (i=0;i<10;i++){
		printf("%-5d", data2[i]);
	}
	printf("\n");

	return 0;
}
```
### using stat print mode, link, size
> lab4-1
```c
int main(void) {
	struct stat buf;

	stat("p1.c", &buf);
	printf("%o, %d, %ld\n", buf.st_mode&0777, buf.st_nlink, buf.st_size);

	return 0;
}
```
### access(filename, _OK), file? Read? Write?
>lab4-2
```c
int main(void) {
	int exist;

	if(access("data1", F_OK) == 0){
		if(access("data1", R_OK | W_OK) == 0)
			printf("R, W, F OK\n");
		else
			printf("F OK\n");
	}
	else
		printf("F Failed.\n");
	
	return 0;
}
```
### symbolic link
> lab4-4
```c
int main(void) {
	char name1[20], name2[20];
	scanf("%s%s", name1, name2);

	symlink(name1, name2);

	return 0;
}
```
### hard link
>lab4-3
```c
int main(void) {
	char name1[20], name2[20];
	scanf("%s%s", name1, name2);

	link(name1, name2);

	return 0;
}
```
### symname, permission, size & realname, permission, size
>lab4-6
```c
int main(void) {
	struct stat buf;
	char symname[20], realname[20];

	scanf("%s", symname);

	lstat(symname, &buf);
	printf("symname : %s\n", symname);
	printf("sym access permission : %o\n", buf.st_mode&0777);
	printf("sym file size : %ld\n", buf.st_size);
	printf("\n\n");
	
	readlink(symname, realname, sizeof(realname));	
	stat(symname, &buf);
	printf("realname : %s\n", realname);
	printf("real access permission : %o\n", buf.st_mode&0777);
	printf("real file size : %d\n", buf.st_size);

	return 0;
}
```
### print file permission, nlink, uid, gid ,size, modified time
>lab4-7
```c
int main(void){
	char f1[512], f2[512], *time;
	int n, i = 0;
	struct stat buf;
	
	n = read(0, f1, 512); // sizeof(f1)보다는 512가 나음
	f1[n-1] ='\0';
	if(n < 0)
		printf("file read failed.\n");

	stat(f1, &buf);

	printf("%o ", buf.st_mode&0777);
	printf("%ld ", (long) buf.st_nlink);
	printf("%ld ", (long) buf.st_uid);
	printf("%ld ", (long) buf.st_gid);
	printf("%ld ", (long) buf.st_size);
	/*time = ctime(&buf.st_mtime);
	while(time[i] != '\n'){
		printf("%c", time[i]);
		i++;
	}*/
	strcpy(f2, ctime(&s.st_mtime));
	n=strlen(f2);
	f2[n-1] = '\0';
	printf("%s ", f2);
	
	printf(" %s\n", f1);
	return 0;
}
```

<!--stackedit_data:
eyJoaXN0b3J5IjpbOTg3MDExMDYwXX0=
-->