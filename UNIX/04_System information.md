# 시스템 정보
### uid 검색
```c
#include <sys/types.h>
#include <unistd.h>
uid_t getuid(void);
uid_t geteuid(void);
```

### guid 검색
```c
#include <sys/types.h>
#include <unistd.h>
gid_t getgid(void);
gid_t getegid(void);
```
<!--stackedit_data:
eyJoaXN0b3J5IjpbNzA5ODYzNzUxXX0=
-->