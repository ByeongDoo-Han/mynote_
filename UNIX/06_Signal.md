# Signal

### signal?

<img src="https://lh3.googleusercontent.com/qeR_xjunXWUTk_tHYb6CTGXDKA9fECLhV_FpHhrhupvjWB7IfvyGw0pMzxKG057fVYAzmWHxmGovZJwbHkMtg99LvodDD8H9xzr8uWMXsNWMmKyiCylSMXO5I_TP4Sp1OT3MOZtGjvMh2CcTswKWncZJ_HNezITSSc8hoSIdtMlejxfW0nF0ZatfT10GXhDbhTyk0VpTAJP2wMDmb0JEsqZ_2DeuI_qs-nW00WZKKcesSIXGR98294F2t4Aw-uSwant7u_0WpDDRDGjqYOIVIyZqCeHoQPq08Xb8Kn884n2ngFG2sTVeuXjDr8AJAg6kdPu9zi02vfZFCSRZzvLcyqFdubnSJjVligTX1IyPmN6CPUpJUM7n-0k4kUxdYN8GhABS0KAvd6rxuIOd3kpfRmvmqviYQy79EinkoNDvafpY0QA8Sp0Y9TXxtfkbSC5ZYSUvzYRH6Ltzc1oDRAi6U-e2qpm1St_jYykcCSlloFnZB0gnlYMWWhtnqX2tpJuk1pmsuazXQWfa2YSNXZNKzyvFZu4KxKXjOsS58gloequ53LwyjwfFe0IooM6C-YVox3FOwXvSxbkvefyaTMxR5wHbNv6nHoZeahORUHDZbg5aur-ItIqwhcuJYRYFLkzvJWtN7eHnR_VcbGL4CfnWzSeG-nAcYh8r3vJ7eVMOOUtbqjEefHTWPlv-5gwKRc9NQiZfzBeQEIENr1r3tMXcRlntetSmm4sWtUfg95uUbicMVO9_=w963-h554-no" width=500px/>

> **signal을 보낸다는 것**은 (B가 signal을 받는다)라는 의미보다 
>  OS에 B의 **signal table**에 mapping된 Action을 B에서 실행함을 의미
> ex) 무한루프 중 Ctrl + c -> OS가 SIGINT를 받음 -> 종료

- software interrupt
- kernel -> process
- process -> process
- 자료 전송보다는 [비정상적인 상황]을 알릴때 사용
- ex) program 수행중 Ctrl + c (interrupt key)
	-> kernel이 문자 감지, 해당 session에 있는 모든 process에게 "SIGINT"라는 signal을 보냄
	-> 모든 process 종료! **그러나, shell process는 무시**
- <signal.h>

## signal의 기본처리
#### 1. 종료 
-> 내 program은 잘 작동하는데 OS의 사정에 의해 종료 (signal에 의한 정상 종료, default)
#### 2. 코어덤프 후 종료 
-> 내 program 상 비정상적인 작동으로 OS가 중단시킴 (signal에 의한 비정상 종료)
	> core file (종료 직전의 momory의 상태)를 생성 후 종료 : debug
#### 3. 중지 
-> 멈춘 상태로 계속 진행가능
#### 4. 무시
<img src="https://lh3.googleusercontent.com/oqLhcStmYo6-q5qgfvOJfJA6KgdkVM-eSMzjTRyJD-Xxl1IBz8sZM4FHF0U_4fyU6RWLuVonao3Pfk39w8ouqCR871irXvXk2gKG6fJyouqIDWgs8f3bmX5DcdBXpuxXfxmDrV9y8s_LEcIjsM-aNG1D_N3x_C7Au3VWtUJVpnxEGnEd12Kbtvo8X_9NVMF-v9mZ3InLETBR0aQQvuW5QBQDTAqK7_P4iigAdI3vJfxy4GqL6T7_aBWTkzakZYJBsNDPVVIwL0S6a-VRZF29DF_FzqqYGBDWpMn5ww1wvypBOQk-RMVzY1q9YpH22S3mXpiByFIyB0WaY1h5_brdQ8vrQ8KqMDHuK0AAnYpZHxI82pW2suTlnBKCwnCH16a2o21qFWoG2K-CCgpMtUq2IaDaN28B4BNBqgrk_uA8yIrQCVol743sqWKXuCV1SY3bLa2DwN2t0z14qir2l-GEvWp0eaxNMlb-N2vW04qeDMVlp3CFBt_y8oxE03lTTZJ2Ad-9_3lOSQbtf0hXJNfYcKIhoAneCRV7GJD-0IE58v5zAa-VjZmWtJCxSlg44noLQLnFAC9Bx3PSSmAGZJvQDmVcjFurhF76dwvVtcuRssu8O_xgsTik5zuOM76h95-D0yS6tvd_BjSvtz_H3Dkz_r9tP5yTnJhvhiq_wqBXfwO6SoGoMZfHU9lwLClefCcHBNG19u2wYL6kg2MuXiKDPpzrlyEhGeWb_8PPs4NxXLT1kY7X=w717-h923-no" width=250px />


### 기본으로 알아두어야할 signal
- SIGUSR1, SIGUSR2
- 그 외의 signal은 process가 특정 상황에 OS가 사용하기 위한 signal

### Signal의 child process의 종료 상태 확인
```c
pid=wait(&status);
if(WIFEXITED(status))	// 정상종료
	printf("%d\n", WEXITSTATUS(status));

if(WIFSIGNALED(status))	//signal을 받고 종료
	printf("%d\n", WTERMSIG(status));
```

## SIGNAL send
```c
#include <sys/types.h>
#include <signal.h>
int kill(pid_t pid, int sig);
```

- pid : signal을 받게될 process 지정;
- sig : 보낼 signal 지정;
- signal을 받을 process 또는 process group 지정
	- pid > 0 : **해당** id의 process에게 signal 전달
	- pid = 0 : sender와 **같은 process group**에 속하는 **모든** process에게 signal 전달. **(sender 자신 포함)**
	- pid = -1 : uid가 **sender의 euid**와 같은 모든 process에게 signal전달. **(sender 자신 포함)**
	- pid < 0 & pid != -1 : process의 group id가 pid의 절대값과 같은 모든 process에게 signal 전달
		> pid = 345 -> pid가 345인 process에게
		> pid = -345 -> group id가 345인 process에게
	- 만약에 다른 사용자의 process에게 signal을 보내면 ? -1 return

		<img src="https://lh3.googleusercontent.com/4_dDQccWPipzY5tet-ohmZgP4zJJDsuie2BWbdfIaqEEFli1OARiI_3cJ32JrTfAdFzbmSwkJTyGKWPbU7e3cqCM1_4o99HB9BoTqfNQdfkocciRHfDpgnTu1FEHgt55xu2OomhDWBPZgHJv5we-QNfCozqh6ZG5kzUg_Kw_8F614CelNqYp14XN9trGDdbZBZTzZ7NGN_4TGRX9ykqrOJLpvv6D6s6WzEiS-Pd76YMQmSNWbAPsRur099EDGXA_sRr5q2Qk6tKwMbhzf0PzscwNlmDrLX_ROhu5Ek5HCsv-pvtaSkmN33PQ1fVgVR72NYOM48dQQ9dHEI15Rgd6_PKUBMiqcDWaq0lM5rSukWvPbiWZ7bGhKfhVu2Gy92a6NlZTqjrrTTPfMfKJeKEOqIlfktqeZfo2MbAkiYYDo2XzD25U1qrHAM_X84rhYuVDPHcIVyiaMdL_el14UTFPITrvH05vaDuYWvYr6w_ljxsEvdfmZGspVavtz2p1TiIIDO2hmeaSxxsCyPWd6JXVWr8xH30BKDDREndY6IIAY0DgUxUVKiqaeUBEx8tV1uxlsySvPF7IFOQPFpWsh4RZUT5N9yk4dw3NDCTkS74ejPyYactkb-NmRdI9E32Qvyv_zfpg8oJ1y3OuWEdzbQhpd1Oar0G0Vh4mrewalk4vOjfAqQ-M4VkuspEnab1ZdOPZoCtx5CTIgwkC4SHAahg9ZUFVIwfhXhjyfhN5obJccOTK6qnB=w963-h889-no" width=400px />
		
		> 위와 같은 예제의 코드로 
		> 1. pid == -1로 실행한 결과 shell까지 튕김(sender포함)
		> 2. pid == 0로 실행한 결과 a.out2만 튕김
		

- 호출 process에게 sig를 보낸다.
	```c
	#include <signal.h>
	int raise(int sig);
	```
	(주기적으로 내가 무슨일을 반복할때 timing을 알려줄때 사용)

## signal Handling

- default action (프로세스 종료)
- 무시
- 정의된 action (sigaction)

### 1. sigaction
```c
#include <signal.h>
int sigaction(int signo, const struct sigaction *act, struct sigaction *oact);
```

<img src="https://lh3.googleusercontent.com/gCVygB0TPqFw_Zj8Ctdt91kSi8DlDCLs4KDXnqmd9k8OaVCGV8b-ypTJuFtCa0gULKYgOdD6bXmkLtYLSR8k1fbujQjW94iThR28xbYN39V9mC2VqMvJGdZ8wDrVMBAuO_YmRQhXU76IgIt-4CQThXaUqNsCfK6vcMf6Wy6qBMbKp7bmOgQgzBZ4Xg5EzDwNnc8O2dtAOB7adaF77ZcyIvn7aqIf2vBG8evJ5IlVMWEhw9IWjVwiyeHGIVL8iof7xJifAcuwZSMTy-SRBOdWAWqORfF1SYAhYK99-3ooWGK7gpmDBHCskh8GVcxuknQZ2QeezJpPLzs2aZsDsn-Rzv1rQLQ29R0r8oIFoyYNrOIPWpTKgmijjpnvCAZiEO7Q1XNy6YJ_ZiSUb4ZDJ950jW6R60f5m1oyll9ykFGpwWHkZ2cRprtD1nfFGt5Af4S6qzLWWfPvdyvL0uq__VjtuebbsPl1yirJoyb36rXZvPCevAWzHTXrNJQlk8KmJnkf5y9j1csZ_g00nWez4lkh6Dy92a6AHdCm0vQHFF-bRKDgskkyNxoorpdK4CYQPi67xXzMYOz_F22WkMW6BXcVHfu35pJhkeNoED2mx5R4MNijUYG74dJWT6FVIT2zt_iXFvY0sl1stiDRnhugp_BFBBssZzqbsCgS3ThVRWDbZNaF2DpXcun2VPH3m0rNQqYU1InUCwwc8bwLTwWPI4i7vpwOJL1quP87Jau_h1FiVAde6Oz-=w963-h528-no" width=500px />

- signal 수신 시 원하는 행동을 취할 수 있도록 한다.
	> 예외) SIGSTOP(process의 일시중단), SIGKILL(process의 종료)의 경우 별도의 action을 취할 수 없다.

#### sigaction의 구조
	```c
	struct sigaction {
		void(*sa_handler)(int);
		sigset_t sa_mask;
		int sa_flags;
		void (*sa_sigaction) (int, siginfo_t *, void *);
	}
	```
1. void (*sa_handler)(int)

	- SIG_DEL (default 행동, 즉 종료 선택);
		
	- SIG_IGN (무시) ;

	- 정의된 함수 (signal을 받으면 함수로 제어 이동; 함수 실행 후, signal을 받기 직전의 처리 문장으로 return);

2. sigset_t sa_mask;
	=> 여기 정의된 signal들은, sa_handler에 의해 지정된 함수가 수행되는 동안 blocking된다.
	
3.  int sa_flags;
	=> SA_RESETHAND : handler로부터 복귀 시 signal action을 SIG_DFL로 재설정;
	=> SA_SIGINFO : sa_handler 대신 sa_sigaction 사용

4. void (*sa_sigaction) (int, siginfo_t *, void *);
	-  sa_sigaction()에 의한 signal handling
	```c
	int main(void) {
		static struct sigaction act;
		act.sa_flags = SA_SIGINFO;
		act.sa_sigaction = handler;
		sigaction(SIGUSR1 &act, NULL);
		...
	}
	void handler(int signo, siginfo_t *sf,ucontext_t *uc){
		printf("%d\n", sf->si_code);
	}
	```
	=> sa_sigaction은 sa_handler와 비슷한 기능을 하지만 조금 더 많은 내용을 표기해 준다.
	
### SIGNAL Handling 예시 code
- signal handling X
<img src="https://lh3.googleusercontent.com/mRFf5H-y6RdVqxYQ7fY4F_mb9HT9YMlBWPolus1yztHhVRNAWbU6sd_ih67SIq3jF-Xi8WRh5VZgtGqmjpRiYWpjoqdsRE9h7lvzbioio8n0fjGOzLtSjvecz8oNxKT0xFK6UaWO0GtUvuBZQRT1KP8UnBdTmnBsGrcJUJaOjZmeiiIa6xoQnE0YJzdtz4hHvbWpWFGzk5q6BSS5dPEq4z5IeIkCIfQ3Y8WkuYy69sQXT5H-G8pi2wkPQmRCHAZQXYppPsZy63n1VVkRMsh7P82UsohZB28hpnCIk-EfdAijRX5CXWBV-6dA_mrLEoTx8E4oxXQ5EOld5MvvJytMXRoOAkFy-7kCraq_b-x8UWoJxqOA44vBEGk7-VGvaPi9IgrgfCa35nAw-s9MXRK3oXpwHZLNFB9kdv0SZ5Zx7znADThlpovl6i7djY7kqYhezKXv8iHOx6sbEUwiClMp6vEtYGSXxjzH4Sc879pC1pmyAy3o87tgT3rXx4SWSezfrauZ9s5Xk62Q4mI-2ZkPeCOT4e5M8aBlL0q_Zb90zXjPVxMjq-pYUF5oJMz8QCVK_BhT8yeTCyUBKfMITjgUEKmk5uN0XqP0XKtGkyiFrRPH-IFq2QaJsQ-ZS-jMCyHG3kP5N0g56IIYgxny6BF4-Mm6Uedvt814VC07EwJN59VMj-h0NkpLpAnv2977KBjbPHWIR4sKaQG-wz3Z6bt0Gz9q9cc3uUdFihOQ5QrlY6uu_sO5=w890-h693-no" width=700px />

- signal Handling O
<img src="https://lh3.googleusercontent.com/TOvBYw1QO9NTX-JxJ8QlABctO0s3lNxnIoCQ0tHIuWOcRgEy2F3gGXWfBZFUEMCgyXnJMr8vPzNrFwpvZlif7z1X7WTPcGC4dsK7djGxQa-MumUrXmPeTznHW-joXmuGrJfFa4grr2lA_EQ0TjNyOj2yNP-XCcokWmZzMhyrTKpTuWhDrkxuriN4NNNbKpBIMywabIUCQOWSGWxXYvjXT8vJPKQUWagKClBaQh3ElswVBLo_FK8aE8In23QsbbnZWeBPot1V8fsDdFtuV8EMqJOy3CY-L9TzZqceDuVbizgnDOUVq10sE6SOErA2W0IvKizPhqr7c1b5wqZFY9yn6cXMdDVaknvAZuh50baFKYE-WovDzOdwf_QVguKcUaktDV_ITZhpZ8xQ49SUUn1lAN5OXoiNg4SPkIKR-l2yw5SMWh9FMEJwKvn0OKtjaGt4UyLY-EvUfPXinhEyatLolY5V29Sp-3ewIPa_u4W_QoUIdqn4sJx1_O4CAn_h28GWDigDPdyZHOEpjCemeIiI5LTo_7y3BxSu7Qq6LAIG83n9LL6XemXK_ieqcnl68-n-QR7BoXxsmc7MBtGJIYUz820E6kfeBQYg9g3pDTBgEGbNRZ8FVPDKgq8Qf_B24ZHG4yfgAE8NHUm_iIn6o3nxbjQO_6nuig-UVnoT1vbkYR1M0_1Y039OtNY5vD-gOnQOKADoc1SZKVyr9zJPqxgDzqPK39hWWJBvFmixXippskt41H8p=w941-h931-no" width=700px />

### Signal 사용 예
1. SIGINT를 무시;
	```c
	act.sa_hander=SIG_DFL;
	siganction(SIGINT, &act, NULL);	
	```
2. SIGINT시 종료;
	```c
	act.sa_hander=SIG_IGN;
	sigaction(SIGINT, &act, NULL);
	```
3. 여러개의 signal을 무시하려면;(1개의 act.sa_handler + 다수의 sigaction)
	```c
	act.sa_handler=SIG_IGN;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	```
> **한 process에서 무시되는 signal은 exec() 후에도 계속 무시된다.**(signal table까지 copy한다. 다만, **함수로 수행하는 경우**에는 함수 주호가 다르기때문에 **수행할 수 없다**.)

## signal 집합 지정
#### Q. signal 처리 중 다른 signal이 올 경우?

1) 다른 signal로 넘어간 후 다시 돌아와 남은 signal을 처리한다.
2) 현재 signal 처리 후 다른 signal을 처리한다.

	`sigset_t sa_mask` -> 나중에 처리, 즉시 처리하기 위한 blocking을 위한

> **blocking**(언젠가 반드시 처리)과 **무시**(pass)는 다른말이다.

- signal 집합 지정:
	- sigemptyset -> sigaddset (빈 sigset에 signal을 추가)
	- sigfillset -> sigdelset(모든 signal을 추가 후 예외 signal은 제외)

	```c
	#include <signal.h>
	int sigemptyset(sigset_t *set);
	int sigfillset(sigset_t *set);

	int sigaddset(sigset_t *set, int signo);
	int sigdelset(sigset *setm int signo);

	int sigismember(sigset_t *set, int signo);
	//-----------------------------------------------
	//ex)
	sigset_t maskt1, mask2;
	
	sigemptyset(&mask1);
	sigaddset(&mask1, SIGINT);
	sigaddset(&mask1, SIGQUIT);

	sigfiilset(&mask2);
	sigdelset(&mask2, SIGCHLD);
	```

### 이전의 설정 복원하기
```c
sigaction(SIGTERM, NULL, &oact); /*과거 설정 저장 */

act.sa_handler = SIG_IGN;
sigaction(SIGTERM, &act, NULL);
// do anything
sigaction(SIGTERM, &oact, NULL);
```

## alarm signal 설정
- timer 사용
```c
#include <signal.h>
unsigned int alarm(unsigned int secs);
```
- secs : 초 단위의 시간; 시간 종료 후 SIGALRM을 보낸다.
- alarm은 exec 후에도 계속 작동; but! fork 후에는 자식 process에 대한 alarm은 작동하지 않는다. (alarm은 program단위가 아니라 process 단위)
- alarm(0); ----> alarm 끄기;
- alarm은 누적되지 않는다. 2번 사용되면, 두 번째 alarm이 대체;
- 두 번째 alarm의 return 값이 첫 alarm의 잔여시간;

> <사용법>
> 1) 몇 초 후에 무슨일을 하겠다.
>2) 몇초 동안 무슨 일이 일어나지 않으면 무슨 일을 하겠다.

## signal blocking
```c
#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
```
- how : (= SIG_SETMASK), set에 있는 signal들을 지금부터 봉쇄
- oset은 봉쇄된 signal들의 현재 mask; 관심 없으면 NULL로 지정;
- how : (= SIG_UNBLOCK); 봉쇄 제거;
<문제상황>
<img src="https://lh3.googleusercontent.com/Nt9W3fE_mpAr1t_HwyaIVGKSZiQUxr-MJl6Dj9H3GR3A-ZhC1pArE8a3n00w3EdmzKWQc4HNwkkvP5n8A-YHu4ZUHCp9wsWP_q9vdeMiS_M35TXsE7VNaattHr_2XUibBhEPI_wqYP-I0jEkUL3qOR0S6k5gbBUp5ufKFQkI8p1o6e8-NqQZ5PBRRCe04eb1HcUwxCokNX3uxjHukvUAmWvlD0-nQbG5yRgv_z1SD49GlPWQK7rymmeVMZv57xt_01PwDu4IG5v3dQwLh42N7Wk68Dnul0625jEQbmIuqcNeM5cDd1gzId4WdZ0ICDovgeV0oz1irKCYESoCdXF7_8SqdR3C9fQBoWqiSN-zRkF3tedD5z1dleDFbmgzbQMF7JCsAq63OCos9BSAFCKZ4I5l-MiuYbyB2UptaMaf8hUaGbBB7jN8UU8frKZFpLxmIj6oM32eCOSPONP39l_pP9lqBEeSWaNjN6gq8L5MMqv1u2KA2VtYiiGC-hHYYTF48udtjluRSQozotnAKgq4sYfOpARqOrpPYVoKu57cPV-ALLelOqk24lnhEV1-xo74OuVLZqePnzddL0A9UMINk7x4ZZ6UYG0kB3za3LnPCeQMe7tloJoNQgJfFpQ3RnvgiAtfp8v-fx6rLCNYxFNy3SRdYCPrcjUZGe-Qdb-lqhuUdDgG4k8fnZuiAQ9iT7XovXBrs-7qCQaZQdWY7OEDdwn83sI14KeSKFKq6nx-m9VtfHFU=w959-h405-no" />
	<문제 상황 => 해결법>
	<img src="https://lh3.googleusercontent.com/BAAOmDcGXZHhkyXGYv_duVQ6yKQcyCEvINTHTWM56WyjRP276OVNJAn5gzLm6yOQWDB9RdFvt8p4nths8w6oKOsaSvF1UvuM3ClnSVzrlcwiNJgneq6b9TEuuxHDaPvBkJT8R9IimwUIiryHPsRq-w1MVFwZnOqnI6Uy31jikCLrNoeh3KepIfQd2wDrL_oE3FnpTp4Xrn3AGMonupuYrlm1lZxMwckOhgEowkFkMrRdELn_pxS2sX0SwI0eXrUNzZe5zfJ7LvsLlRS54EjZ7vgqE1nGOMY7Y6QxldLOVae0vmREIaMf2-TBp9D1-_OAraAmnGtJppsPNTAnTywXu_lOmFsSxn9plGBGUHhSi1oLJ6Tfp0LHnOOUa6NJaNOCHRiF84sMO55st9pyAmje22cM9cYo68bN3NDdhVS3-rebg_p1GXlZJr_1bH7Cc8hCATMWg-TGxGseht58d0Atc-J8OIxq2Hc2V3uHxW19D6W_2lhXVWNDguSnVbcFobeNIRIShgEDX6U_qbDhs4Wz4Ec1opZA2jzwQ8a57w8TcdJu1Zlbivq_KB59PqytPJo3MWMpAwLgCG0GQC7CrZcfwgtqYTBcKvA97jmuUSGrT8hGSA0As9dewrrAGd3KgFrjYHfdUkqwjnVusiVTCXAoStaOZCGOOP6O2S7ssc5pApMt22AR6jO0Nr9h0gKA6xnJ2Y9uVdUpD_ZWT4NYWbhgrrLmUmNJuFu1NRUbRo0AGAPgmoEz=w619-h943-no" width=500px/>
### pause System call
```c
#include <unistd.h>
int pause(void);
```
- signal 도착까지 실행을 일시 중단(CPU 사용없이);
- signal이(어떤 signal 상관없이) 포착되면; 처리 routine 수행 & -1 return;

<!--stackedit_data:
eyJoaXN0b3J5IjpbMTAwOTgwNzkzNF19
-->