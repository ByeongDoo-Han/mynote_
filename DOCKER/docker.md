# Docker 정보 확인
## Docker 버전 확인(docker version)
docker의 client와 sever의 버전을 확인하고 Go언어, OS, Architecture을 확인가능하다.
## Docker 실행 환경(docker info)
![](https://lh3.googleusercontent.com/AnrCBaS8wMP1IXUPqtzikjg3kcUxgGswCz0vdcAwy6r7uHvITRDkNt9OMcK9eGIBl_FDfA4KYNBgOSazjzt5idiSEqPfdFbabp2UCrZwm-8I52SNdhmvoIBC6QlUyLhHTgnOU21YsZznPybZFTcj4h89xOTM_AG1T_kI_LSy5M_EjvR-MI1Il_JCjcNkvWPcy_bunMUnHEYbQqeQQJDOAkBBxocsudHWCKHp7VcGjIMNCO_DolcQZjnSc3PJrDcRxm91nYbiK5mdHfuPF9hWS03NY0FD9dmlP3DAfp8jZcqeGH_wazIvsJXTo_nmAb5W9C4BLuOIuAFyzAX2QTKrPRKmr22dK0pGz2p16f7I9-89Zuku4HVHOFx4TKOeEwXeT3gX9D5D0iPVOrqz_QnjoMlgjuGMsdXCLxsNOLYaWY4sBQwiJhitVWACzDGwpZRUYUBhyDBI7IuYKJHLEbwrtf9qF5sf2IBz5Dvz3Lip-sngommXssg10e-fNWlS8Mlg7KpW_srwPvxEyMaw63HAzBy-uo5NdSUEd_tv_dfRLxtZaAjpbDt-rKxurOPMGAfd-bwF4oRxEuRJkRIwPZj0yngEb5Dmwww-uCwcRvEe6mi1279_bGZnEEQuLYQWr4BUki0wrshRWAo6Y5rlq6VoP9qOSkg7NRXOnqAYO5VrgErnmPuNsOwvmo5JZbde3BiEPu_MZW3MjlrYE8Cu1Npyn4Pa=w849-h943-no)



--------------------------------------------------------------------------------------



# Docker image 공유
## Private registry 구축 및 관리

- 사용 이유 : 팀 개발 간에 새롭게 생성한 Docker 이미지를 공유하는 경우에 멤버간 동일한 환경에서 애플리케이션 개발을 하는 것이 중요하고, Docker 이미지를 중앙에서 관리하는 레지스트리를 Local 환경에서 구축하고 이를 관리하기 위해
-  Docker Hub에 공식적으로 공개된 registry를 사용한다.(https://hub.docker.com/_/registry/)
- version 0은 파이썬, version 2는 Go언어로 구현. 대부분은 version 2를 사용한다.

1. `docker search` 커맨드로 registry를 확인

		$ docker search registry
		NAME                                DESCRIPTION                                     STARS               OFFICIAL            AUTOMATED
		registry                            The Docker Registry 2.0 implementation for s…   2630                [OK]
		distribution/registry               WARNING: NOT the registry official image!!! …   57                                      [OK]

2. Registry 이미지 다운로드

		$ docker pull registry:2.0
		
3. 다운로드한 registry image 확인

		$ docker images registry
		REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
		registry            2.0                 3bccd459597f        4 years ago         549MB
		
4. registry image를 기반으로 `docker run` 커맨드를 통해 registry 컨테이너 구동(포트넘버 5000)

		$ docker run -d -p 5000:5000 registry:2.0
		65ceed38cc08d684d3247ef01fad29631d65a579d937e1fbe434076b195fe682
		
5. 컨테이너 확인

		$ docker ps --format="{{.ID}}\t{{.Image}}\t{{.Ports}}"
		65ceed38cc08    registry:2.0    0.0.0.0:5000->5000/tcp

6. 업로드할 이미지 생성 및 build

		# Docker image
		FROM centos:latest

		# maintainer
		MAINTAINER 0.1 your-name@your-domain.com

		# Apache httpd install
		RUN ["yum", "install", "-y", "httpd"]

		# Apache httpd execution
		CMD ["/usr/sbin/httpd", "-D", "FOREGROUND"]

		$ docker build -t webserver

7. private network에 업로드하기 위한 **tagging**
	><형식>
	> docker tag <로컬 이미지> [Docker repository의 ID address | 호스트명:port number]/[이미지명]

		$ docker images
		REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
		webserver           latest              7743267ec55d        38 seconds ago      340MB
		registry            2.0                 3bccd459597f        4 years ago         549MB

8. <local -> private>로 이미지 업로드

		$ docker push localhost:5000/httpd
		The push refers to repository [localhost:5000/httpd]
		9403a4ccd015: Pushed
		d69483a6face: Pushed
		latest: digest: sha256:b9ddc98796c8904bec38f43ae79d5618e72b614e8b41ebf5a37395d7dd8d64e1 size: 4740

9. 기존 local image 삭제

		$ docker rmi webserver
		$ docker rmi localhost:5000/httpd

10. <private registry -> local> 환경으로 다운 

		$ docker pull localhost:5000/httpd
		Using default tag: latest
		latest: Pulling from httpd
		8ba884070f61: Already exists
		48b08652a3d9: Pull complete
		Digest: sha256:b9ddc98796c8904bec38f43ae79d5618e72b614e8b41ebf5a37395d7dd8d64e1
		Status: Downloaded newer image for localhost:5000/httpd:latest

11. 다운받은 이미지 확인

		$ docker images
		REPOSITORY             TAG                 IMAGE ID            CREATED             SIZE
		localhost:5000/httpd   latest              7743267ec55d        5 minutes ago       340MB

---
# 여러 컨테이너 통합 관리 - Docker Compose

### 3계층 웹 시스템 아키텍처(3-Tier Web Application Architecture)
- Front Server
- Application Server
- DB Server

## Docker 컨테이너간 링크
Docker 컨테이너로 3계층 웹 시스템 아키텍처를 구성할 때에는 여러 컨테이너가 필요하다.
여러 컨테이너를 연계할 때 Docker 링크 기능을 사용

	컨테이너 링크 설정
	
	$ docker run --link <접속하고자 하는 컨테이너명:alias명><이미지명><실행 커맨드>

ex)
	
1. PostgreSQL 공식 이미지인 postgres를 사용 DB 서버 기능을 가진 컨테이너 구동

		$ docker run -d --name dbserver postgres

2. CentOS 이미지를 사용한 웹 애플리케이션 서버를 생성

		$ docker run -it --name webserver --link dbserver:pg centos /bin/bash

3. **appserver**의 환경변수를 확인, **dbserver**로 링크된 prefix <PG_>를 확인

		[root@01f7789093c2 /]# set | grep PG
		PG_ENV_GOSU_VERSION=1.11
		PG_ENV_LANG=en_US.utf8
		PG_ENV_PGDATA=/var/lib/postgresql/data
		PG_ENV_PG_MAJOR=11
		PG_ENV_PG_VERSION=11.4-1.pgdg90+1
		PG_NAME=/appserver/pg
		PG_PORT=tcp://172.17.0.2:5432
		PG_PORT_5432_TCP=tcp://172.17.0.2:5432
		PG_PORT_5432_TCP_ADDR=172.17.0.2
		PG_PORT_5432_TCP_PORT=5432
		PG_PORT_5432_TCP_PROTO=tcp

	PG_PORT_5432_TCP_ADDR은 dbserver에 할당된 IP Address를 의미한다.

4. **appserver 컨테이너**에서 **dbserver 컨테이너**로 ping test 하면

		[root@01f7789093c2 /]# ping $PG_PORT_5432_TCP_ADDR
		PING 172.17.0.2 (172.17.0.2) 56(84) bytes of data.
		64 bytes from 172.17.0.2: icmp_seq=1 ttl=64 time=0.082 ms
		64 bytes from 172.17.0.2: icmp_seq=2 ttl=64 time=0.076 ms
		64 bytes from 172.17.0.2: icmp_seq=3 ttl=64 time=0.196 ms
		64 bytes from 172.17.0.2: icmp_seq=4 ttl=64 time=0.055 ms
		64 bytes from 172.17.0.2: icmp_seq=5 ttl=64 time=0.074 ms
		64 bytes from 172.17.0.2: icmp_seq=6 ttl=64 time=0.061 ms
		64 bytes from 172.17.0.2: icmp_seq=7 ttl=64 time=0.070 m

	appserver 컨테이너의 /etc/hosts에 pg라는 이름의 alias로 dbserver 컨테이너 네트워크 정보가 있음을 알 수 있고, ping test도 가능하다.

		[root@01f7789093c2 /]# cat /etc/hosts
		127.0.0.1       localhost
		::1     localhost ip6-localhost ip6-loopback
		fe00::0 ip6-localnet
		ff00::0 ip6-mcastprefix
		ff02::1 ip6-allnodes
		ff02::2 ip6-allrouters
		172.17.0.2      pg 5ee62d32e9f6 dbserver
		172.17.0.3      01f7789093c2
		[root@01f7789093c2 /]# ping pg
		PING pg (172.17.0.2) 56(84) bytes of data.
		64 bytes from pg (172.17.0.2): icmp_seq=1 ttl=64 time=0.060 ms
		64 bytes from pg (172.17.0.2): icmp_seq=2 ttl=64 time=0.075 ms

## 구성 파일 (docker-compose.yml)
> docker-compose 구동환경은 docker 상 CentOS 이미지로 컨테이너를 만들어서 linux version으로 진행하였다. [참고 : centOS에서 docker daemon install](https://docs.docker.com/install/linux/docker-ce/centos/)
> 
>  docker-compose github ([https://github.com/docker/compose/releases](https://github.com/docker/compose/releases))
>  

**YAML은**~~strikethrough text~~
구조화된 데이터를 표현하기 위한 데이터 포맷으로 들여쓰기시 `Tap`이 아닌 `Space Bar`을 사용
### 베이스 이미지 지정(image/build)

- 이미지 태그 없는 경우
	```
	webserver:
	   image: ubuntu
	```
- Docker Hub의 이미지 지정하는 경우
	```
	webserver:
	   image: iamjjanga/dockersample:1.0
	```

- Dockerfile에 이미지를 구성하고 이를 base image로 하여 build하는 경우

	디렉터리 구성
	```
	sample
	├ docker-sample.yml
	└ Dockerfile
	```

	build 설정
	```
	webserver:
	   build: .
	```
   
	Dockerfile
	```
	FROM ubuntu
	```

- 컨테이너 생성 `docker-compose up`
	![](https://lh3.googleusercontent.com/a_5zk9WbVekINHoKsIwV0gIpgB3z6EYlAO412-QI4sbsLHV93Vqc05uTy1M2tLSPG2udJFKCkbVreUlXmVefIfmY7QDLpM-UOBzygZTODxn4cHklIEU6bauf6xR3zcCeaaWEo_gq4W9pBg6A17JpAqw_KlNfLWHupx0VtFiJRp6uDHqr8Vwu6jxPsf2TiMXz_HYc1kK__EeIZdv43GxVPrzSj8g-xzkBgyl_Xqbl8rcUkP1dIGu1rZGWTVNydVHpDWTa-b6EusFknEvFv070ZOJMEdFxnc0G4v_vZ6CmXwWyNeHeXfVULRBYQxynS3QyRPlRl8qcf-II37DPdPTEI8dYrog0_XVm2ygZ3Ya61w4CZyU5s35diwhXBmZ8Gpu3M58-fo2TmNQuV1xuGgzY7QJB-g_izMRCcHqCJBw7DaSE1qZUcsIqmMXAIm--wkrkoKzihhmZ2ObKHDyTGlJTTCZR-nepWk0V-PAZ7ELpeeJvX96HFIE1yY0dXqWhn_hG9n9Ro0ef2IhBpqhzn39KltB1xS2Ai3ralj64EvUS7l8otmODRYqAufqk_ghbB4mol3euJVUJeD5IhfLpRy0EPONRU9k9vElYzCgBy3lVseBqzCCdrmEDI_ayasi15cA8lAmD8pvnzUFR5sN_3V3fWzwKSIq020oMzLqu_rgdI3ZOUbOfIb1NKzgh6uf54vi2PGJrb-QtYU68DQ5LM8qE5fiE=w923-h392-no)

### 컨테이너 내에서 동작하는 커맨드 지정(Command)

	command: /bin/bash

### 컨테이너 간 링크 연계(links/external_links)

**'서비스명:alias명'을 사용할 수 있다.**

- 링크 설정
```
links:
   - dbserver
   - dbserver:mysql
```

- /etc/hosts 내용
```
172.17.2.186 dbserver
172.17.2.186 mysql
```

**external_link**는 외부에 위치한 다른 컨테이너와 링크시 사용. (이미 동작하고 있는 데이터 전용 컨테이너 등을 사용할 때 활용)

```
external_links:
   - redis
   - project_db:mysql
```

### 컨테이너간 통신(ports/expose)
컨테이너가 공개한 포트는 ports에서
1. `Host machine` port-> `Container` port
2. `Container` port만 (`Host` port는 Random)

`YAML`은 xx:yy 형식은 시간을 나타냄으로 port 기입시에는 ""를 꼭 적어주어야한다.

- 공개 포트 지정
```
ports:
   - "3000"
   - "8000:8000"
   - "49100:22"
   - "127.0.0.1:8001:8001"
```

포트 번호를 Host machine에는 공개하지 않고 링크로 연계된 Container에만 공개할 경우에는 `expose`명령어를 사용한다.
(DB 서버와 같이 Host machine에서 직접 액세스하지 않고 웹 어플리케이션 버서를 통해 액세스 하는 경우에 많이 사용)
- 컨테이너 내부에만 공개하느 포트 지정
```
expose:
   - "3000"
   - "8000"
```

### 컨테이너 데이터 관리(volumes/ volumes_from)
컨테이너에 볼륨을 마운트할때 사용

Host에서 마운트하는 경로를 지정하는 경우 'Host dir : Container dir'

- 볼륨 지정
```
volumes:
   - /var/lib/mysql
   - cache/:/tmp/caches
   ```

볼륨을 읽기전용(Read Only)로 하는경우 `ro`를 붙인다.

- 읽기 전용 볼륨 지정
```
volumes:
   - ~/configs:/etc/configs/:ro
   ```

다른 컨테이너에서 모든 볼륨을 마운트할경우 `volumes_from`

- 모든 불륨 마운트 지정
```
volumes_from
   - log
   ```

### 컨테이너 환경 변수 지정(environment)
- 환경 변수 지정
```
# 배열 형식으로 지정
environment:
   - HOGE=fuga
   - FOO

# hash 형식으로 지정
environment:
   HoGE:fuga
   FOO:
```
설정하고자 하는 환경변수가 많을 경우 환경 변수를 정의한 다른 환경변수 파일을 읽어 들일 수 있다.

- 환경변수 파일 들이기
```
env_file:envfile
```
- 여러 개의 환경 파일 들이기
```
env_file:
   - ./envfile1
   - ./app/envfile2
   - /tmp/envfile3
```

### 컨테이너 정보 설정(container_name / labels)
생성하는 컨테이너의 이름을 붙일 수 있다.

- 컨테이너 명지정
```
container_name : web-container
```
컨테이너의 라벨을 붙이는 경우

- 컨테이너 라벨 설정
```
# 배열 형식 지정
labels:
- "com.example.description=Accounting webapp"
- "com.example.department=Finance"

# hash 형식으로 지정
labels:
com.example.description:  "Accounting webapp"
com.example.department: "Finance"
```

설정한 Label을 확인할때는 `docker inspect`로 확인

## Docker Compose 커맨드

| Sub command | Descriptions |
|--|--|
| up | 컨테이너 생성 및 구동 |
| scale | 생성할 컨테이너 개수 지정 |
| ps | 컨테이너 목록 확인 |
| logs | 컨테이너 로그 출력 |
| run | 컨테이너 실행 |
| start \\ stop \\ restart | 컨테이너 구동 \\ 중지 \\ 재기동 |
| kill | 실행중인 컨테이너 강제 종료 |
| rm | 컨테이너 삭제 |

`docker-compose` 커맨드는 'docker-compose.yml'이 저장된 디렉터리에서 실행된다.
다른 디렉터리에서 'docker-compose.yml'을 실행해야 하는 경우 `-f`옵션으로 파일 경로를 지정한다.

- docker-compose.yml을 기반으로 컨테이너 생성 및 구동
```
$ docker-compose -f ./sample/docker-compose.yml up
```

- 특정 컨테이너 중지
```
$ docker-compose stop dbserver
```

### 여러개의 컨테이너를 한번에 생성(up)
docker-compose.yml을 기반으로 여러 컨테이너를 생성하고 구동할 때 사용

	docker-compose.yml [option] [service]

|options| description |
|--|--|
| -d | 백그라운드에서 실행 |
| --no-deps | 링크된 서비스를 구동하지 않음 |
| --no-build | 이미지를 빌드하지 않음 |
| -t, --timeout | 컨테이너 타임아웃 시간 지정(default 10s) |

ex)
아래와 같은 docker-compose.yml을 구성
serverA는 Apache HTTP Server
serverB는 MySQL이 base image

- docker-compose.yml
```
serverA:
    image: httpd

serverB:
    image: mysql
```

docker-compose.yml에 정의된 두개의 컨테이너를 구동하기 위해서 다음 커맨드를 실행 `docker-compose up`

- 여러 컨테이너 백그라운드 실행
```
$ docker-compose up -d
```
![](https://lh3.googleusercontent.com/vPMsrk97_dRdrdQGtkKCmxrFzsl1bt4GjN2SRZ6gdNDs6FM4p5ipVpIumgsgXQfz6kkvxf497bI49rS0Wohl83sQvYZQey2Sz69zLB8WH91FrT9aWjxDdayNurqwKju4MAd_7O1ez2d-zs5pXYrl4V6yeL2O4OB614ox4EM5OeaYhxbLwJupfW1hfo5MTPn-7VdURo9qREkDGM_YriTMf7IdTL5elIXoAeQduuD0kQifDOU9janjKRrCvvpZiUxMgYu-W2eZFyMsXjIGnpag4K8dZAOIFDMa4VK6bPQj5avgBmREHNlJf5L1HA41RBH_TT4_CjmxTAYrv0LPjXd611a0E3TDQNiI4Rgptdry5O4GiJa38Vzuqxl8_1w0L3UTYs2d3JjQhX-g58thH0NWSrgxlc6YkE-Mx4MVBOdZRnrm5Jjjy__KB-AGjWe_LYXJQq2DvmVayo0n2cM89HSnYLHbtR38lKqzjUC9PmSg2d2QyfnmJ1zbxzY75h6_Mbop9qcF_iyVltVQfa8liKc3mqrrbE3MGUKmGmwMU-EPadgYG6Aes8lN6M7q3eBFN_NofUE71xtHRaIRJaGFZpfkXn_j663_HkyM184CiApntGavvk9-EiOs_L97nrCPe7WHwTwKYb-_fduP91PxNVr-SDiVj7MT8ZS21RggZ7EZiGZWpCmmQrkWNCVeQprPPsa3k4jg_p7u5mQqhXUoAf4m9bUR=w1758-h943-no)
### 생성할 컨테이너 개수 지정(scale)
```
docker-compose scale [ 서비스명=개수 ]
```

docker-compose.yml에 serverA와 serverB가 정의되어 있고, A는 10개 B는 20개 구동하고자 할때

- 컨테이너 개수 지정
```
$ docker-compose scale serverA=10 serverB=20
```
![](https://lh3.googleusercontent.com/iRUFsyUGz8nFS-EhODAYaMQyf6EoEyp0BRYEWmtiz7Vdk4XVb1-waG8C3pBsCeCAKzR5HQQFvTQCcqU-vNeIQDaSdYj4eMeHDpAm78HMcLkp_Rr8EPpOw0soQeC8xir7QpjBNVrN2f7fUToRVgP9MGb5OkPWcUChtsUHV7XzFO1LYB5SSOz0fDtaV9DGgPs7hqbgesxB5SsuHxur7GarERRSgOSsNGAR_o3t9eZtoCDnB0b7MyIpL0ON7iiZx0ovr6-2cxmJEyHz8GTxR03BLI-xCSCtUfYp8zSvTKageZi64LldWAiqqfEJ2ktB0I5SOMtyhS0gdIQtW-7Wb32vHIyKwOQi4V-2wIaodhJqEk9tafBGbPjyBTWi0RRG-8RCpvSq6IS8xZqsbUr_8VCUMWBfEPQtkw7E_8rJ28NHsEzcI46GYdL9HKv2gZnMt0YEH7n32_i964x4J4gKoWhFNsQ6LOv6SBi6N_3iAhPggyHgY_exven1pNyagBZ0ELdEmpXf83yQTmreaZCcFZ6sTrJlWjDkrXz-J4Fbvb8jPDSbU-e0kAa5_t_cmEKrrWLOqkhH6usk32ShiFqd0Pxa3Wr1iAv_5NJ7DMG6f6H7ACs01_MBLuwPgWcSNEX6R6yoWRSwo-4fMzumjEyEp3-kDmohomej6p9HOU2c043_peHmzNTf2Qlgm53Le4w8763DL_gRUho-EaLdN_y9yAos5mMT=w1857-h481-no)

### 여러 컨테이너 확인(ps / logs)
- 여러 컨테이너의 목록 확인
```
[root@197f158d68d9 sample]# docker-compose ps
      Name                   Command             State    Ports
----------------------------------------------------------------
sample_serverA_1   httpd-foreground              Up       80/tcp
sample_serverA_2   httpd-foreground              Up       80/tcp
sample_serverB_1   docker-entrypoint.sh mysqld   Exit 1
sample_serverB_2   docker-entrypoint.sh mysqld   Exit 1
sample_serverB_3   docker-entrypoint.sh mysqld   Exit 1
sample_serverB_4   docker-entrypoint.sh mysqld   Exit 1
sample_serverB_5   docker-entrypoint.sh mysqld   Exit 1
```
- 컨테이너 ID만 확인할 경우 `-q`옵션 이용
```
[root@197f158d68d9 sample]# docker-compose ps -q
871a54ac210cca67f81492b6d547697dbcf193feabefe6f5be44fa68c13369a3
fabe0fb74863f00cbbd4a865b1eb7802af51e6d43ac06a5d639d0b6a32a1ebce
b67ac255bda3072847c9f30932d62f59d11e2af98b30de9a81e2b55a88cd3e97
a9e33f9496b6d11c4b0cff63ec58568e55eadc04ea7c0b4eea64a8f9989ed33d
66206cece2bf5e0febce1acc5c69dcc4717953a6a9b873ed58f7333ff0638e5b
5fa3352d6bf740e2e36348a70a59ac4cbb1143a23c6d9b470adc6c4ad96200dd
054897660f3f83e8206f68e5afe75d89ca0f490e41687ad2fecd3ef5f14e749e
```
- 로그 확인
```
$ docker-compose logs
```
![](https://lh3.googleusercontent.com/z9R7kaXMWF0Jp5PMs8csRWpmnacWAbXubdSY4vIYDQvrmYSM5ZR-9oj9OWeQtNOl3lKbj2T1gr7FzE1nHEC8oPW-jb-42z5bUcx9GrDXEM2aqMhICh2fLJ6JSKXTJEz8cwnh9kwq5mpwqV7Rx_RZDjws-jlht6dfAxg-uv_EpZ1aOVf0DIJFMIpagDaLlLNfaVHAKABwTRBYeQ1a-87N8GC78yGspHW9EaXEItJQXIhvb6wdrWY8MahBEJa7dyaHjpGcL_FDNoa8ktpkp4xTCBiF70sbHUmrQ8YLibFdghiEDKLV4pp96l5mSTLTxGTyGWbcFf85W7WNok1V1jY3X2gYeRmPUzWdIG79RX9TDBPMuqsnsD6hCn94GzPRqyJKkWt5KccX2YO-lb-6B3yuixWYsBT-FiKw6fuMG6DwO3yPQVnPywWPYFdavdBFl-1P9cdZJfvhrZzP1ieNbKAohbChU7f2eLMWu3EL7taIMQ8WXhOYROiv69v5hGVXiC1ttrkhWSK3CvlYYwTwO0sdmgUNLJthyfJdAcXBCssxaCH8dZIFZ-Gc1P55fr4GCvgScZ1PFc3wJcIAuQkDyT-unuSCdy7V5cPzNyPH8upUNwSUoAGgO9MPCmzwgjh6xtmdA-j3dHOyB3zcSNLZQNAOjCLTN9vp7qyKsqb_N0Oosu0BHULQqmItdpA4-W7MSMqfLAzTLKxcSS0A_X7cM0M38EJa=w1440-h461-no)

### 컨테이너에서 커맨드 실행(run)
docker-compose에서 구동한 컨테이너에서 커맨드를 실행하고자 할 경우 사용
```
[root@197f158d68d9 sample]# docker-compose run serverA /bin/bash
root@77e8190f0e4f:/usr/local/apache2#
```

### 여러 컨테이너 강제 종료 및 삭제 (kill / rm)
실행 중인 컨테이너 강제 중지 -> `docker-compose kill`
- 컨테이너에 시그널 보내기
```
[root@197f158d68d9 sample]# docker-compose kill -s SIGINT
Killing sample_serverA_2 ... done
Killing sample_serverA_1 ... done
```
위의 커맨드를 실행하면 컨테이너에 키보드 interupt인 `SIGNIT` 시그널이 보내지며 `ctrl` + `C`를 입력한 것과 동일하게 작동한다.

옵션 없이 `kill`을 보내면 process를 강제 종료하는 `SIGKILL`를 보냄

- 여러 컨테이너 한 번에 삭제
```
[root@197f158d68d9 sample]# docker-compose rm
Going to remove sample_serverA_run_f382637250a9, sample_serverB_4, sample_serverB_2, sample_serverB_5, sample_serverB_3, sample_serverA_2, sample_serverB_1, sample_serverA_1
Are you sure? [yN] y
Removing sample_serverA_run_f382637250a9 ... done
Removing sample_serverB_4                ... done
Removing sample_serverB_2                ... done
Removing sample_serverB_5                ... done
Removing sample_serverB_3                ... done
Removing sample_serverA_2                ... done
Removing sample_serverB_1                ... done
Removing sample_serverA_1                ... done
```
`-f` 옵션으로 확인 메시지 없이 바로 삭제할 수 있다.

## Docker Compose를 사용한 WordPress System 구축
>**WordPress**?
> 오픈소스 기반의 블로그 구축 플랫폼으로 PHP로 개발되었고 MySQL을 사용한다. PHP와 HTML 코드 수정 없이 다시 정리할 수 있는 위젯이 포함되어 있고 THEME도 설치해 자유롭게 전환 할 수 있다.

구성
: 웹서버(webserver 컨테이너)
: DB 서버(dbserver 컨테이너)
: 데이터 전용 컨테이너(dataonly) - 특정 서버 프로세스를 동작시키지 않고 데이터를 저장

### 데이터 전용 컨테이너 생성
dataonly 컨테이너는 BusyBox의 이미지를 사용하여 생성한다.

> **BusyBox**?
>  표준 Linux 커맨드를 하나의 binaray 파일에 저장한 애플리케이션. 가전 제품, 네트워크 장비, 모바일 장치 등의 임베디드 기기에서 자주 사용

- Dockerfile 작성
```
[root@197f158d68d9 sample]# cat Dockerfile
# Bring Docker image
FROM busybox

# Maintainer
MAINTAINER 0.1 your-name@your-domain.com

# Data
VOLUME /var/lib/mysql
```
1. BusyBox를 이용하여 webserver 이미지를 생성하고
2. 작성자 정보 입력
3.  VOLUME 명령을 통해 데이터를 저장할 곳을 지정한다. (MOUNT)

- 데이터 전용 컨테이너 생성(dataonly)
```
[root@197f158d68d9 sample]# docker build -t dataonly .
Sending build context to Docker daemon  3.072kB
Step 1/3 : FROM busybox
latest: Pulling from library/busybox
ee153a04d683: Pull complete
Digest: sha256:9f1003c480699be56815db0f8146ad2e22efea85129b5b5983d0e0fb52d9ab70
Status: Downloaded newer image for busybox:latest
 ---> db8ee88ad75f
Step 2/3 : MAINTAINER 0.1 your-name@your-domain.com
 ---> Running in 72275ab9fdc8
Removing intermediate container 72275ab9fdc8
 ---> 42ec3939eb15
Step 3/3 : VOLUME /var/lib/mysql
 ---> Running in 873f59d6f500
Removing intermediate container 873f59d6f500
 ---> 1805dbebc840
Successfully built 1805dbebc840
Successfully tagged dataonly:latest
```

`docker images` 명령어로 dataonly 컨테이너의 크기를 확인
```
[root@197f158d68d9 sample]# docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
dataonly            latest              1805dbebc840        10 minutes ago      1.22MB
```
약 1.2MB로 작은 용량의 컨테이너가 생성되었음을 확인할 수 있다.

- 데이터 전용 컨테이너 구동
```
[root@197f158d68d9 sample]# docker run -it --name dataonly dataonly
/ # ls
bin   dev   etc   home  proc  root  sys   tmp   usr   var
/ # exit
```

- 컨테이너 상태 확인
```
[root@197f158d68d9 sample]# docker ps -a
CONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS                           PORTS               NAMES
fe553875cf37        dataonly            "sh"                     16 seconds ago      Exited (0) 5 seconds ago                             dataonly
```

### 웹 서버와 DB 서버용 컨테이너 생성
1. Web Server
WordPress의 FrontEnd 기능을 제공하는 컨테이너. 
80번 포트(HTTP)를 통하여 사용자의 request를 받아 처리하는 역할을 담당.
Apache HTTP Server가 동작하며 PHP로 동적처리를 수행.
또한 백엔드의 DB 서버와 연계하여 데이터 처리.

2. DB Server
WordPress의 생성된 데이터를 관리하는 컨테이너.
MySQL이 동작하며 Front의 웹 서버에서 수행된 결과를 처리한다.

Web server와 DB server의 구성 정보를 docker-compose.yml에 작성

- docker-compose.yml 작성
```
[root@197f158d68d9 sample]# cat docker-compose.yml
# 1. web server config
webserver:
   # 1-1. image
   image: wordpress

   # 1-2. port
   ports:
    -  "80:80"

   # 1-3. container link
   links:
    - "dbserver:mysql"

# 2. DB server config
dbserver:
   # 2-1. image
   image: mysql

   # 2-2. data storage space
   volumes_from:
    - dataonly

   # 2-3. env
   environment:
    MYSQL_ROOT_PASSWORD: password
```

1. **웹 서버 설정** - 웹 서버 컨테이너의 이름을 webserver로 설정

	1-1.  이미지 지정 - WordPress 이미지 사용
	
	1-2. 포트 설정 - "Host : Container" format으로 HTTP 80번 사용
	
	1-3. 컨테이너 링크 - "연계할 컨테이너명:alias명" format으로 db server 컨테이너와 연계

2. **DB 서버 설정** - DB 서버 컨테이너의 이름을 dbserver로 설정

	2-1.  이미지 지정 - mySQL 공식 이미지 사용

	2-2. 데이터 저장 장소 지정 - Mount할 컨테이너명을 volumes_from에 입력

	2-3. 환경 변수 설정
	> **MySQL 이미지에 지정할 수 있는 환경 변수** 

	| env | description |
	|---------|---------|
	| MYSQL_ROOT_PASSWORD | MySQL 관리자 패스워드 |
	| MYSQL_DATABASE | MySQL 데이터베이스 작성 |
	| MYSQL_USER | MySQL 사용자명 |
	| MYSQL_PASSWORD | MySQL 사용자 패스워드 |
	| MYSQL_ALLOW_EMPTY_PASSWORD | 빈 MySQL ROOT PASSWORD를 사용할 것인가 여부 |

### 컨테이너 구동과 데이터 확인
- 백그라운드에서 컨테이너 구동
```
[root@197f158d68d9 sample]# docker-compose up -d
Pulling webserver (wordpress:)...
latest: Pulling from library/wordpress
f5d23c7fed46: Already exists
4f36b8588ea0: Pull complete
57a069704459: Pull complete
...
0d7f34f2c87d: Pull complete
Digest: sha256:fdecb6fc92b04d88419544722ac1679158c12eb8f519b83b0480a6375e823dec
Status: Downloaded newer image for wordpress:latest
Creating sample_dbserver_1 ... done
Recreating sample_webserver_1 ... done
```
- 컨테이너 상태 확인
```
[root@197f158d68d9 sample]# docker-compose ps
       Name                     Command               State          Ports
---------------------------------------------------------------------------------
sample_dbserver_1    docker-entrypoint.sh mysqld      Up      3306/tcp, 33060/tcp
sample_webserver_1   docker-entrypoint.sh apach ...   Up      0.0.0.0:80->80/tcp
```
동작을 확인 하기 위해서 Host의 docker-machine( 필자는 default)의 IP address로 액세스 후 확인
> **여기서 막힌점**
> docker상에서 centOS로 container 만들 당시 port forwarding을 하지 않은 관계로 생성된 컨테이너로 바깥 Host의 IP로 Access 하더라도 접속이 되지않는 것으로 판단. -> 다시해볼것

- 데이터 전용 컨테이너 구동 및 확인
```
[root@197f158d68d9 sample]# docker start -ia dataonly
/ # ls /var/lib/mysql
#innodb_temp        ca.pem              ibdata1             public_key.pem
auto.cnf            client-cert.pem     ibtmp1              server-cert.pem
binlog.000001       client-key.pem      mysql               server-key.pem
binlog.000002       ib_buffer_pool      mysql.ibd           sys
binlog.index        ib_logfile0         performance_schema  undo_001
ca-key.pem          ib_logfile1         private_key.pem     undo_002
```

/var/lib/mysql 디렉터리가 Mount 되었음을 확인할 수 있다.

### 데이터 전용 컨테이너 백업 및 복구

`docker export` 커맨드로 데이터 전용 컨테이너(dataonly 컨테이너)를 `tar` 파일로 압축할 수 있다.
다른 호스트 머신으로 `tar` 파일을 옮기고 컨테이너를 구동할 수 있다는 장점이 있어 데이터 전용 컨테이너의 이동성을 확보할 수 있다.

- 데이터 전용 컨테이너 백업
```
[root@197f158d68d9 sample]# docker export dataonly > backup.tar
[root@197f158d68d9 sample]# ls
Dockerfile  backup.tar  docker-compose.yml
```

- 데이터 복구
```
$ tar xvf backup.tar
```


<!--stackedit_data:
eyJoaXN0b3J5IjpbLTU3ODk1OTE1NywxNDUxNzcxMjQsMjM2Nz
c1ODUsNzM5OTMxMDAzLDE0OTIzNDEzMTksLTEzMTU3MzcxNTdd
fQ==
-->