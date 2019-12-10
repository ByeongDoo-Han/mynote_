# Docker 설치
설치 환경) windows 10 pro, hyper-v
설정 환경)
1. Hyper-v 활성화

제어판 -> '앱 및 기능' -> 프로그램 및 기능 -> windows 기능 사용/사용 안 함 -> Hyper-v 확인 클릭

2. Hyper-v 가상 네트워크 설정

Hyper-v manager -> '가상 스위치 관리자' -> 새 가상 네트워크 스위치 -> 외부 -> 가상 스위치만들기 -> 이름 및 외부 네트워크로 설정 -> 확인 -> 네트워킹 변경 내용 적용 -> '예'

> *막혔던 점 : docker-machine create 과정에서 `docker-machine create --driver hyperv default` 명령어를 실행했을때, iso 파일의 다운이 느렸던 점이 있었는데 다른 날이 다운을 받아보니 정상작동함. link에 많은 사용량이 있거나 사용 네트워크 상태가 좋지 못했던 걸로 예상한다.

3.  Tera-term을 다운 받아서 사용
docker-machine create 후에 `docker-machine ssh default`를 통해 ssh 설정 -> 설정된 Ip address과 port number 22을 Tera-term을 입력
> username : docker
> password : tcuser
> (>에서 $로 바뀌면 성공)

4. Docker 작동 확인

`docker run <docker images> <command>`를 통해 'Hello world'를 출력해본다.
`$ docker run ubuntu:latest /bin/echo 'Hello world'`를 실행하여 확인한다.
![Imgur](https://i.imgur.com/45BE53X.png)


**docker-machine의 재실행과정에서 'Timeout'이 발생한 경우(active가 없어짐)**
![docker-machine timeout solution](https://i.imgur.com/5pzK7ob.png)
생성한 docker-machine을 stop & start를 후 다시 env를 세팅해준다. 

**docker-machine list에서 Docker을 찾을 수없고 'Unable to query docker version'이 발생한 경우**

![](https://lh3.googleusercontent.com/xeZ_qw0vk7keyPOq8FTI5-9lLVWroJIO4hFIUrl0rJ8C50vnNzFQSBx_dIkA2MGWd34Nlzilb2yYeM3suyrqVeiVLqYNOEBQEX0WYrqVBMAT_QA-jUE3atusRaw5x-O9AyxCh4VEG6tHYPO_dCrb669yhhL7sCDXvVZckPl_v5hdSa38wF-rNzP5epqJF4_lZt5fOQpuCOZuB1J-KtVZQZ4NTm_p_2JYV0NdBp6QM5UlfPeP4VHEMwYNl1M7Ij7Wjb_fMT2MZ-H09hFaYBU7dT9voqgwG2oHpo1g7-r3PSDYS-nY4OKsrZKwaVh3FlGrcyjA8daKT0p0afT_KxP-zsi5mpxgeVpp_9nZuNuyQTlQt6UDctikCxNtM1LSM--sN29y_yHCZDXZnOzXSA7qXWpRx_u-evgZK8yqx0jg3p-DKf5f_mR8UlACihaASE05in7TIdeJnWvy2lo5BzgyRs-d4KsIt0Wr4B2HhrI9TVjZhccJZ6nEAweJ0yZQeBPx8LzP9yeaCt3lPVBfJsovEFLqPrcXs7eU7eeEJJRUrOaDDVLN6q7_FB7H0NN7l3jQ_goa-lYPq2tcFkhIe5sb8BiH5wmvDe1Jj78WINvsvErU_MBMLaqXDKe3h6vIQJfhgybU06WHE1E353hRMAJaL5DRwwwdhQ76fkgyShcm6RE7HG--5ee1vZoen8OsXoZ0oIN6kR1tNTGIDx49yzbcziPK=w960-h418-no)


# Docker 명령어
## 이미지 다운로드(docker pull)

`docker pull [options] <image>:<tag>`

**지정한 이미지의 tag를 다운**
`$ docker pull centos:7`

**이미지의 모든 tag를 다운**
`$ docker pull -a centos`

**다운로드 URL을 지정하여 이미지 다운**
`$ docker pull registry.hub.docker.com/centos:7`


## 이미지 목록 출력(docker image)

`docker images [options] [repository]`
| options | description |
|--|--|
| -a, --all=false | 모든 이미지 표시 |
| --digests=false | digest 표시 |
| --no-trunc=false | 모든 결과 표시 |
| -q, --quiet | 이미지 ID만 표시 |

>ex)
>`$ docker images`
> 로컬 환경의 모든 이미지 출력
> `$ docker images --digests iamjjanga/dockersample`
> 로컬 환경 docker 이미지 digest 표시

### note. 이미지 스푸핑과 수정방지
Docker Content Trust(DCT) 기능을 이용하여 Docker 이미지의 진위 여부를 확인한다.

**서명**
-> 이미지 생성자는 docker push 전 로컬 환경에서 이미지 생성자의 비밀키로 셔명 (Offline key)

**인증**
-> 서명된 이미지를 다운로드(docker pull)할 때, 이미지 생성자의 공개키로 이미지의 진위 여부를 한다.

> DCT key enable
> `export Docker_Content_Trust=1`
> DCT key disable
> `export Docker_Content_Trust=0`
> 공부 중에는 error가 뜨는 환경을 방지하기위해 disable하고 작업한다.

## 이미지 세부 정보 확인(docker inspect)

`docker inspect [options] < container | images | ID >`
출력하는 세부 정보 내용 : image ID, Created, Docker Version, Author, Architecture etc..

**특정 항목만 확인**
`docker inspect --format="{{ .Os}}" centos`


## 이미지 태그 설정(docker tag)

`docker tag <changed image>:[tag] <new images>:[new tag]`

기존 이미지를 rename 한경우 **name과 tag만 변경될뿐** image ID, Created, SIZE는 변동이 없다.

> **docker image 명명 규칙**
> <Docker hub 사용자명> / 이미지명 : [태그명]

**rename 형식의 tagging**
`docker tag httpd:latest iamjjanga/webserver:1.0`![Imgur](https://i.imgur.com/wHi27lR.png)


## 이미지 검색(docker search)

`docker search [options] <search keyword>`
| options | description |
|--|--|
| --automated=false | Automated Build만 표시 |
| no-trunc=false | 모든 결과 표시 |
| -s, --stars=n | n개 이상의 별 수만 표시 |

**별 수 50개 이상 찾기**
`docker search --stars=50 centos`
![Imgur](https://i.imgur.com/KhbxQCf.png)

## 이미지 삭제(docker rmi)

`docker rmi [options] <image>`
| options | description |
|--|--|
| -f, --force=false | 이미지 강제 삭제 |
| --no-prune=false | 태그 없는 부모 이미지 삭제 X |

`docker rmi httpd`
![Imgur](https://i.imgur.com/B6x6Gxf.png)

## Docker hub에 로그인 & 로그아웃 (docker login & logout)
> `$ docker login`
>  Username : ~~
> Password : ~~
> ...
> Login Succeeded
> `$ docker logout`

## 이미지 업로드(docker push)
업로드는 Docker hub에 login한 상태에서 실행한다.

`docker push iamjjanga/webserver:1.0`![Imgur](https://i.imgur.com/rCGYdxF.png)

**Docker Hub에서 확인**
![Imgur](https://i.imgur.com/yT5XIGf.png)



## Docker Run

### 컨테이너 생성 및 실행(docker run)
`docker run [options] <image>:[tag] [value]`
| options | description |
|--|--|
| -a, --attach=[STDIN,STDOUT, STDERR] | 표준 입력, 출력, 에러 출력을 연결 |
| --cidfile="filename" | 컨테이너 ID를 파일로 출력 |
| -d, --detach=false | 컨테이너를 생성하여 백그라운드에서 실행 |
| -i, --interactive=false | 컨테이너를 표준 입력 열기 |
| -t, --tty=false | tty(단말 디바이스)를 사용 |
| --name | 컨테이너명 |

**/bin/cal 커맨드를 실행한 캘린더 표시**
`docker run -it --name "test1" centos /bin/cal`![Imgur](https://i.imgur.com/mwiwUU2.png)

**/bin/bash 실행**
`docker run -it --name "test2" centos /bin/bash`![Imgur](https://i.imgur.com/Z00iKOw.png)

**note** 프롬프트 사용자 -> 관리자 권한 변경
![Imgur](https://i.imgur.com/zm20KVf.png)


### 컨테이너 백그라운드 실행(docker run)

`docker run [options] <image>:[tag] [value]`
| options | description |
|--|--|
| -d, --detach | 백그라운드에서 실행 |
| -u, --user="username" | 사용자명 입력 |
| --restart=[ no, on-failure, on-failure=n, always ] | 커맨드 실행 결과에 따라 재가동 |
| --rm | 커맨드 실행 완료 후 컨테이너 자동 삭제 |

* `no` : do not automatically restart the container(default)
*  `on-failure` : restart the container if it exits due to an error, which manifests as a non-zero exit code.
* This flag also accepts an optional restart count : `--restart=on-failure:5`
* `unless-stopped` : restart the container unless it is explicitly stopped or Docker itself is stopped or restarted,
* `always` : always restart the container if it stops.
 [From. Commnly Used Commands] (https://rampart81.github.io/post/docker_commands/)

ex) `docker run -d centos /bin/ping localhost`

![Imgur](https://i.imgur.com/1K0czbV.png)

### 컨테이너 네트워크 설정(docker run)
`docker run [options] <image>:[tag] <value>`
| options | description |
|--|--|
| --add-host=[host:IP Addr] | 컨테이너의 /etc/host에 hostname과 IP Address를 설정 |
| --dns=[IP Addr] | DNS 서버의 IP Address 설정 |
| --expose=[port num] | 포트 번호 할당 |
| --mac-address=[MAC Addr] | 컨테이너의 MAC Address 설정 |
| --net=[bridge, none, container\:\<nameid>, host ] | 컨테이너 네트워크 설정 |
| -h, --hostname="hostname" | 컨테이너의 호스트명 설정 |
| -P, --publish-all=[true, false] | 임의의 포트를 컨테이너에 할당 |
| -p [host port num]:[container port num] | 호스트와 컨테이너의 포트 mapping |
| --link=[container:alias] | 다른 컨테이너에서 액세스 시 이름 설정 |

**컨테이너 포트 매핑**
`docker run -d -p 8080:80 httpd`
-> httpd 이미지를 기반으로 컨테이너를 생성하여 백그라운드에서 실행
![Imgur](https://i.imgur.com/HSfFCax.png)

**컨테이너 DNS 서버**
`docker run --dns=192.168.1.1 httpd`

**MAC Address 설정**
`docker run -it --mac-address="92:d0:c6:0a:29:33" centos`후에 다음과 같이 확인할 수 있다.

> \* **중간 centos에서 ifconfig 명령어를 찾을 수 없다고 하는 경우**
`sudo su`로 관리자 권한 변경 후 `yum install -y net-tools`를 통해 package를 다운받고 다시 명령어를 실행해 본다'

![Imgur](https://i.imgur.com/FHzP7JQ.png)

#### docker run 커맨드로 호스트명과 IP Address 설정
`docker run -it --hostname=www.test.com --add-host=node1.test.com:192.168.1.1 centos`
컨테이너의 bash에서 /etc/hosts 내용을 출력하여 다음과 같이 확인한다.
![Imgur](https://i.imgur.com/x7q2fom.png)

**`net` options**
* `bridge` : bridge 접속(default) 사용
* `none` : 네트워크에 접속하지 않음
* `container:[name| id]` : 다른 컨테이너의 네트워크를 사용
* `host` : 컨테이너가 호스트OS의 네트워크 사용


### 리소스를 설정하여 컨테이너 생성 및 실행(docker run)
CPU와 Memory 등 리소소를 이용하여 컨테이너 생성
`docker run [resource option] <images>:[tag][value]`
| options | description |
|--|--|
| -c, --cpu-shares=0 | CPU 리소스 분배 |
| -m, --memory=[memory usages] | 메모리 사용량 제한(단위 b,k,m,g 등) |
| -v, --volume=[host dir]:[container dir] | 호스트와 컨테이너의 디렉터리 공유 |

ex) `docker run --cpu-shares=512 --memory=512m centos`
CPU default는 1024, 이를 반만 사용하고 memory도 512 mb로 제한
-> Linux의 cgroups 기능을 사용

ex) `docker run -v /c/Users/LeeSiHyeong/test:/tmp centos`
Host OS의 test directory를 컨테이너의 tmp directory와 공유

### 컨테이너 생성 및 구동 환경 설정(docker run)

컨테이너 환경변수 및 컨테이너 내의 작업 디렉터리 등을 설정하여 컨테이너 생성
`docker run [환경설정 option]<image>:[tag][value]`

| options | description |
|--|--|
| -e, --env=[환경변수] | 환경변수 설정|
| --env-file=[filename] | 파일에서 환경변수 설정 |
| --privileged=[true \| false] | privileged 모드에서 구동(호스트의 커널 기능도 사용 가능) |
| --read-only=[true \| false ] | 컨테이너의 파일 시스템을 read-only로 설정 |
| -w, --workdir=[경로] | 컨테이너의 작업 디렉터리를 설정 |

ex) `docker run -it -e foo=var centos /bin/bash`
환경변수 설정
![](https://lh3.googleusercontent.com/qgbaFmuoWDwaVoCfyCSVD0m1oLzgkg8NEEc7e-dOWT731hhsdRJry7jU1rxY6s5I4BnMcuuD64pGwlviG6XJzmwDoT6c5L13R0Y19xCi22DmQ3UEFnH2-Uoq2O9ER-jz2FmuGA4A_GGZkrnvgaRfbszMxbtSBtSZGrkysPTsLDNIDiR5jgZxpD9cUk1lAZdjA6QAtPYwafefY0SPB2hrUSuLyCC-V-Av_VTC3fnQYKvo5jN4iQy1OqAk89fiuGHOTc9dQWh2LSIYddPh33B8J4PdN_LE90YNPW9WWzozqA2WWO2cW1XcuF1gFwe5UjwmRJsc2IqosA1uPF6XqK7vay_JjAhSXRKdEi-v1rY7yIbQKxX9zEXCbyQpKbEwHPGtoiLj8rugBzIxL49NMpXzdq7jf4sE1G2yvpxoIx2nQCjyoG3a7LNO8tNxm5QRR3TxoKbirzH6DwhkIZ_3UIgngEEBxdvNRpe13MNTA5_a_y3YW68XY5e2Kf8fOi11jxF5yOaHCRBg5kmouPKEtVOnHbbuHkV00LH5GITfX5-jizUfmgkqqthhfAukaxRLCLYHS-NDcPw38NvTGOZfo2EzA2LLbDAZlbp10KBTPSCdxQuASvZWgsS-Qf-9LyOXwg3cDuAwKyfkB9jUnHZkfOI9vc0Tea88VMOnnQuogVFckYd9jAAYm4jYyVL32sCNOJ8lJ__XZ6TPKtSCAaHyFCft12QH=w1052-h317-no)

ex)`docker run -it --env-file=env_list centos /bin/bash`
환경변수를 설정한 파일을 일괄 등록
![](https://lh3.googleusercontent.com/ktiOnemkzmWJ7nFso-VDLBinTbGllfUuNEVy6mH-3koGcz-LbS2VaAw1QT7ROXpIsRGrt0Plm74ONgiCU64iHI5CXHLHqmGEgMSCgL5MpdUKhBlIyXz7JV0wLWu-rd3I3d5DSxUtutKe302XiU4NRqFN9g8GCpg9rSxMTz93lepk1seF-lP7Te-yq4sDSzxhmRJuxnZBbDyFwVhNJfTxubaC4kkqZ7nmWiOp_sLCaOQaAOfCBY3CPhZYi_tHzGP9AWIDly0JSLhHAVq8vQh2Ii-zY8Afr9MbuHiFOpd6ZQ0EMH27iYJpiybvn62gzSGmDbAPEHgGEHcxpOiK_U_K2glYffQg3xuXuLfBatUZakIMYLXErGkwRekoWHp1h00lkHT1gYlkb-B5AIQlm1WG_F3Du6NCYqN5_OI8mrba5vYQGGEpI4PFbnORbshl_FFsG41DZOjMuU7NRa2Q9EiPwq7AKAaWGFqSS0c7EYeWlNAK4aAqN9l7Cw-nAFq4oVG77GlehhyLveimXbmfOHB8WFGIT3uLhLLvnoAq7Q-AY-_x9LNbkJK-wLgem6_vavz-h32l2j2OlBvszsAXUkBDruOchbrAs697ytxj5bUu0ioGEid4HVtwgN-GSOKDmL4LdwPiIkBnG-NzGlE-5gROsTJeTSYrRekpsCGQ6fmU9SV1w9XA4AMgex37Z_jGo6MBRPctsWag2VdQ58wFk2zoQLdJ=w1052-h519-no)

ex) `docker run -it -w=/tmp/work centos /bin/bash`
container의 working directory를 설정하여 생성
![](https://lh3.googleusercontent.com/mZa7M7UNxg1eBTZcUI5xbGhezsxMLTQDwKcsEu-sldzNvrCKHKdyhdo1Ex8x-GAfxaMhKUTYhhIsSSxpi8PT3ydMEmvncrmYZGM6CLTZQ26nTxF08BGILK6EoWAaIlo4bdZyuZ-QJ9we4VdljTEgBO6_AOxZdPd7eDadRqTvaZOLogwmSJDuHj0iPRdj27NLeGSPtoc518_0nqiTF0cr8W20JANzDpDznqxNYx9TB3LYv1A_mQhf4G0CyTDCfGGVvUp4sTHm_Jqbs3MCYbosfxnzvX2tQ7N59UczzhoS6X6_Isjl0V9w2_1caVwUKERTv0dj_k3WNhSboeBPUFWMNDJYq5OqcpVGcCbobDyN3c6YEqOZduLrlEpOTPrH8xHkwAjV1xF5ZLsqWtcg_6pFvyei0Arx-8Pm3QM9dUHg8-Qvk5kE2_xIx9J6PxyqhDhCVd8u4-bcZr465e0hadK6Y-lcegV2BXOgqsv7JebbU6X--r5lut3l8JCQHC1cqh-lV9TYnl77YloUX7FeSthcl3fWLr3nWmeZIXbkMp5Py36WuGDYvrTTM5zYEDVgjXiIHhSOQRUZT_tLaHqW2g1Ng1bTxFfLGIlrqqNH1Q1kauTfw2nOkI7mftVAD5eEeyA23zajy6KKbXHTbniUWq07DwLQItXewJd7IGQTt46IHxGFLy2TCjnaIdRokg5JZZO5WyNi85yYF9TlfRNSGWggdidk=w1052-h106-no)


## 컨테이너 목록 확인(docker ps)
구동 중인 컨테이너 목록 확인
`docker ps [option]`

| options | description |
|--|--|
| -a, --all=false | 구동, 중지 상태의 모든 컨테이너 표시 |
| --before="" | 입력한 컨테이너명 또는 ID보다 이전에 구동된 컨테이너를 표시 |
| --since="" | 입력한 컨테이너명 또는 ID보다 이후에 구동된 컨테이너를 표시 |
| -f, --filter '[key]=[value]' | 목록에 표시할 컨테이너를 필터링 |
| --format '[key]=[value]' | 목록에 표시할 포맷을 설정 |
| -l, --latest=false | 마지막에 구동된 컨테이너를 표시 |
| --no-trunc=false | 생략된 정보 없이 모두 표시 |
| -q, --quiet=false | 컨테이너 ID만 표시 |
| -s, --size=false | 파일 사이즈를 표시 |

ex) `docker ps -a -f 'name=test1'`
이름이 test1인 컨테이너 필터링
ex) `docker ps -a -f 'exited=0'`
종료 코드가 0인 컨테이너 피터링
![](https://lh3.googleusercontent.com/ONksiMVpvUjTvJOjpw-eeS4uj8Tb8oJBFhNivadChPURcaL6BEpCT8mO5Gayel7CxAhPmWdVfsYQoUo2zXu6JwH3nSSM-FJqNZnGIIJ8NlwaMvlPvIPGnotmqoyu--ywk-Ntyrjc9-dDgBibGOZGCcU8zIPgl3EWz8GuCFtd6OG5pQcZaIWAelTY-brewqbpvP5_r1PnmMTpEeWozM1kKEuaQ3YWMsfnrVlBEdpFwlLqgAhI9aedaS6z_J-HYop4q0Ck65xk3EegEma4e5sqwdVFssHhjrb0ssG6JMBfCSlCs6d89d49ck767gTfuJ_frHFEUsx6QHCDfa_oX9gZdNr9UpVK1NPKiEkphbIb1FIrP0eNbQIYlHmBPKUSGKz7qwwyzAlBRk7VE3_18tyO_Beb-ol_zgd5tg-r5zAIXIObNhtMP-OJpiGe46ZIG8aiI_1hZCVDAMAI1T8t22SwAUK5Yw7Jh6dfUyoGp2uOKufPTcvSQ1l9QVH97CYvREuEOi2ymU7u9szCllfTyhSgiG7rDUg5MlZDVBfSql2RV4Gq0eMzyDi9cLEnfp7SWXGVtgONi6mmBr4Sm5hNZa9_WoXUqF-CgI12nAxclyq9bXQkG4ChLTxCg_ChskEY4AU8DvjV2Vuo-_fofxs8ut9tdsAgeqqA9h4q_UdlmFyTcPEZ3F5KmXmb1L3hbyIdcPo0MCvvW2Vd0Rw0AsvRxMtA3PzV=w1916-h328-no)

[표] **출력형식**
| placeholer | description |
| -- | -- |
| .ID | 컨테이너 ID |
| .Image | 컨테이너의 기반이 된 이미지 |
| .Command | 실행 커맨드 |
| .CreatedAt | 커맨드가 생성된 시간 |
| .RunningFor | 컨테이너 구동 시간 |
| .Ports | 할당된 포트 |
| .Status | 컨테이너 상태 |
| .Size | 컨테이너 디스크 사이즈 |
| .Labels | 컨테이너의 모든 라벨 |
| .Label | 컨테이너 라벨 |

ex) `docker ps -a --format "{{.ID}}:{{.Status}}"`
컨테이너 ID와 상태를 출력
ex) `docker ps -a --format "table {{.ID}}\t{{.Status}}"`
컨테이너 ID와 상태를 table 형식으로 출력
![](https://lh3.googleusercontent.com/78PQmHm2fjUNcLjYHqDwQwYJixEN1xnju2XUn27IeMLpd-ser3xWs-xV5g0ABU46QpDzrBw8v3gEkzNwumFNtKrWv8udpQdes7usXWSTYH6hBxrH-yiyozH7h0_9IYul8HJcof-_Uwj-g4mHcP6HGfr0ztqGYcmvbX5fs3UpZiwh4-r15ELBXKS_Rs-S1PxzAA9ygJZlIIgxu1nwqvmgzI72qLTliZt53d2TErm9mQMjW9wJzZb5zdEeWbCdS5t4of-idMYFDEzRNiCrDM6jxZ-l2fwcfiQOpVqxGdsr4JH2vVFuhW9PcXhi4pKHbet7KxTMUFx6tCtDyFs43_C0-N9gktXSP_hw0SJmrE2oMCi243-M2d4PEypnT9aCoA6qQCq1NDadULngLwvq4uBUAsmGB4pth5dyJzpzKinFl7XM870q5yRgRYGD0PnYDqvosAhm_0NqV1xe0dOxtAac6dJs2-pSx1BkOoTCkSvuFw75ndeMRjuBsyXGdVnUFZuqLrbXfCtzoaLh26EzqEHos-RwjyJp7SA1lOS3HoZ0Dj4bGRC5Pfh6EkGYyypV1bnxed4IKW4lc_UqtQVZfN00-lAxoTA6WIkZBWoLaTpSAblQOjAvA0XusCav5lWFRMUH0pBAqqhTfnPQCZvrLYfvnPojV8edoHGzGdjrlonHxvvUnryum13TZ2FmpAAXlrOVslmVUqEcazdHunHt-Y-ogsvg=w1916-h657-no)

## 컨테이너 구동 확인(docker stats)
동작하는 Container의 상태를 확인
`docker stats <container | ID >`

ex)`docker stats ID ID`
![](https://lh3.googleusercontent.com/8T_v3gfFSpsXjHogHLo_8ypCnGC5rgJTVtRuvwOduzOaI6pgmkxnTzBPshXuBUVUmafWpUYcBuXma3eMeCcKXg9MK-dqswjvhmC28QDe6Gug2R_4V4r4keDqhoWsU9bHPQ-ry28jiGSUJlkiM4YR3kbKbMpTOdfl8ytR4-ScJQ-0QPLFKzgQOuob5Iw6Z4FN3iwWFPP6I8YbZj79n5DF12YlT5auM7KpglUa7xdb4H1TdxUD8gfiB-wlCjeuGM66tdxvxJtLXmTgV6ZWhIIiT0wKGXgGOFH5sIghFbgrXOkGZiQs0ky1J_0PSjj1XZEtaiTs7Ce3hnD4P0GFbSr6F6CpRuL2dYw_rL0HIQx1-05b1-VxFyzmytJHWZWF_VRYO6aCjsaNtYYI-Ftp_-pd6EGmEEhPTlk2ODDfwzpg52Aazzwp6wB32VMUL-INUBEIwRzFdeX8nQ2mVUEjtovQVTFUlbwI1_pVkzMsuP9Su1X2w22uPmWmIRRRyWp73ZbOHJOon7Y9A7fjb_9oyxPHisWCnNvjWXyfO7hX3b2YdVtzo4fsFkRns0fAuNWH6SM7za35gaXqee8N73JGF-HdQUqa-l-fGL5DLGgA88_wuK7OvFSwxGVFYGmVZQxi9_T-CfuAAKj_f18wk6kWc0TJmSkh0_B-l652MBKBXHZPJGN4I_U5fsjYeI2vHJaeXeDOaxzNZJrnblsO1NvwzYRdghOl=w1916-h90-no)

## 컨테이너 구동(docker start)
`docker start [option] <container | ID>`

| options | description |
|--|--|
| -a, --attach=false | 표준 출력, 표준 에러율 연결 |
| -i, --interactive-false | 컨테이너 표준 입력을 연결 |

## 컨테이너 중지(docker stop)
`docker stop [option] <container | ID>`

| options | description |
|--|--|
| -t, --time=10 | 컨테이너 중지 시간을 지정(default는 10초) |

## 컨테이너 재시작(docker restart)
`docker restart [option] <container  | ID>`

| options | description |
|--|--|
| -t, --time=10 | 컨테이너 재시작 시간을 지정(default는 10초) |

>컨테이너에서 실행되는 커맨드의 종료 상태(정상 종료인지 아닌지)에 따라 컨테이너를 자동 재시작하고자 하는 경우
-> **docker run --restart option을 사용**

## 컨테이너 삭제(docker rm)
`docker rm [option] <container | ID>`
->**docker rmi와 헷갈리지 않게 주의!!!**

| options | description |
|--|--|
| -f, --force=false | 구동 중인 컨테이너를 강제 삭제 |
| -v, --volumes=false | 할당된 볼륨을 삭제 |

ex) ``docker rm `docker ps -a -q` ``(`'`가 아닌 ``  ` ``임에 유의)
>모든 컨테이너를 삭제(docker ps와 조합), 추가로 구동중인 컨테이너 까지 삭제시 `-f` 옵션 사용

## 컨테이너 일시정지 및 재시작(docker pause / docker unpause)

`docker pause|unpause <container | ID>`

![](https://lh3.googleusercontent.com/JQ6UdwMtTAWOGEb8vaNc7VetlGLyS1HIgl8J7_PYFxLq6n77hLEc6FeR91PA2oc5BxM_Oos3UcK0637Vz2NRQdrtdud-mE1c2hCzIu9feS6OSFti3WR1pGux93XOxrxKuVlrYmeyQ049sktdaRZBEU6kUFXjOXWRvjdE42Sq6E4e50RuY4QaOIdbuaHNYMQYtFrnLHACafhRBl1CmqMoN4G1rhvBtGkox3M9bRePt_f3sk9bnysvqOmJLwzv98YzufTGouwhX5Vk4ViRc2hNSUx3apyc3D0gvT6Lg2ReFq7cnAU_49lXvDwlRdZ0keSJbBECVXKbcWOFarYpi5oYVfB-pHSMkdyZAhxErwXEwEqKTQafXkLc4D85Wise9NSnbw_CIM69I6XTeu7cDfES_qe-i7qzADuzXMuY4qyctT-r8-5sJr-XfpBJYU0PyyLSgRq_vqvKB35BJbKaKjTX2fFvco9gDvqpqozmL98jhYVEanljCV0fbMof5MV6Rdaus03l7q0vmZpm60PNkbmNjPweL_QHA9o6x37c36KXaUDl91KhYJH40ekPxG1CufN2M35p-1_FucBYYoY594rD-YrfbK-iRSf79dsqLSstqI0Ayp9NZf6Lhf1vi8EI5ztqqyCmMNeOPycNMRebelgKPHhfLF0mvV6pI1jYTN6AOy28ogTZ6v7HvOVIfXE76wleQGmty3pskEh4bxbriSNiPx50=w1436-h294-no)

# Docker 컨테이너 사용법
## 컨테이너 접속(docker attach)
`docker attach <container | ID >`

![](https://lh3.googleusercontent.com/7fEBG0xN4PQQgapfK9jPBmXDNZTpRIWaQT-rFxV18m5ZfLxdTM1OGOy6vZ9N91ySfd7ty3HSNwXpyIxXCojpy7ri4syr0Io43LvGNTvJJFqcGQI2xO6oHXz23nWpUyijr-jmCgsg7N2U1dquz4Cq--76bYN0GtzxnWlVqNd1jJTwIhLz2QcGe00gNjbFTYYgCJzDqxyNPOTNxZ1kTeKRYwjehinpFmWuvZC0G26PkVwVtX6UqYHJvIiuszEsrOxCaKeiYu551n_tkYNgA6qt_D8_ThXZYQCVpatqfJp7gxEkDFLFik_P_r29iCKV5HlueWpxtghsloj2Zr67sdyBlIySC6P19pqYuQZSRiw_cFSIiSWLkXHYuhTIUV09QV-mLIkZ4NNXDphQdTwjNnETjlOP7t6LwM7tFXYTSYwx-jIXk4dSPbm1QE98LYmIIq3SRgolNqAQYqVP5_t2GpRg2K50Fp-slok8jwCQ8dVSlNUqSRqzgTQ8VwQGpChlL9NsTIbhhRn0uXu2D1v11zGD92ElPZMGuQBlIlafbjH1gpxwN9weT-aPfS8otQFAtonDV4y5ZtMERIbSkpVSd3FXVgvi9CTAhJeB0AxZpWi-eqvt9KCc4c1mVfHZFGrxojs156LvYJJ-vlKdJJyTTz-2V5jUIDd17sN4zGTnKNfhd_Py3e0HfCWeQZ2DgO52SomYdmyRsog0dNsdKq169alHud4S=w952-h193-no)
->`ctrl+c`에 의한 컨테이너 종료가 아닌, `ctrl+p`,`ctrl+q`를 통한 프로세스를 빠져나와 상태를 유지할 수 있다.

## 컨테이너의 프로세스 실행(docker exec)
구동 중인 컨테이너에서 새로운 프로세스 실행시 사용
-> 백그라운드에서 실행하는 컨테이너에 docker attach로 접속할 수 없는 경우에 많이 쓰인다.
`docker exec [option] <container | ID> <command> [value]`
| options | description |
|--|--|
| -d, --detach=false | 커맨드를 백그라운드에서 실행 |
| -i, --interactive-false | 컨테이너 표준 입력 연결 |
| -t, --tty=false | tty(단말디바이스 사용) |

![](https://lh3.googleusercontent.com/bF8taugnPoV4Mp-9vj8zMjPF3nF4x_aLJel0U5pOm8jn9icu48zTxYXgZ0G7Av8V7oc19CTP5RUstcQHl56wIB7FJzOoTbkXg-NB61PMSsb8TjVcnAa7_VYa7snxU62vd1lSAAkcP7S-lHY1oWclLzI0OopOKRkco-14-yhsIRD_lGCmBHnnzhVHaGLezWi-AHdTn4d_9b_Q-lP8WMdtOOUElwk9C30h5K9pKmxzy_T4A06eLO4Q2xf1vguHVg9UnRvv0fFjUOqEscxNqOt2goqlhTJ5gtTgS6ul0dSj25fTBOMk3LwZWPAlSbvWHLOXWMejsZjOTnHAZ37-kYxqJq_C-RcLDRKg78Y0hlqyQ4IPcW3J6qJ4PXCiWsPn1_p0HQAsySPdL2qdHQ154ZILutDoM0v-sGxcSAupaqCVoHBpdeZlhOHsCIGbd5jnmOsxdsXhHWbbRd0xQxSn3lpurIj8kiOHGys50eP7rFsvz76xOTThAr90UojAfOZUQlyVTQcfmeGbp0AXqWFi5DvZZYvP8pcHJkJSEzukQYh3YP2C60nHKvoxmxprXPfh02AVFsmMaw9xgYqNI-_dvimKylrIRirMLPKBuMHmJAi56HlPW9mNBUcScenX65D-yU-J0XrkaJ5F_CvTbMbbuZ5BxyJjfoX-YA2F7gQH-SbxlK5bLlIW7fx4h4H6vTJnG2sDJMz1StRQAAG79pYre-N5Mpy1=w952-h183-no)
- docker exec는 구동 중인 컨테이너에서만 실행가능,
- 중지된 컨테이너는 docker start 사용

## 컨테이너의 프로세스 확인(docker top)
구동 중인 컨테이너에서 실행 중인 프로세스 확인
`docker top <container>`
![](https://lh3.googleusercontent.com/kpCiV3R3FL3yAKMRbx7zDSCTzRidcHY3Mm5sQQ-eedIMh-StGXdYbPW8AaZ4NTHMMzm8diXqevpt3447uONHdMiuWNdgw8P7mknpCMY71l757qFSRiSGE5oMIqjgUv6EMK8W_aVNlKxFyzpOPUb6s9nMkPlr48CasPm0KccSDI6IlYNx7bGgnknoPJqwfrfsMuDXADCaervWbyRog8_R9_pG8gn2_k55hu2FdUgtrbyEVQVfSqc80vyqtg3iodDlac6F_2vREoDvpr6XxtTtMpyH_j9moGRellMG7qKzz7FDd1bI3JIPUriefipcpiTEj77ICaZqwwr7prhSKfIYZoFeksCPqFTNl3XR3dzO4G72mj3XTi9jFpt35tO4mTZo4hmosWbJaVI363neCeGDR9jzwU0lVUREm30y1K1Y885i_oVTKp7VNLNRJCMUnRh6jLxcW9J8C_GIqxEqfLPHx-4MRxaR4It86mWVpUbq6qgOiEjtYf0Cgqg1icQ7reHecMk22YgTmNlBTtIgI_ZgKBOaYGYUmALqKmsju7kJOgNtobRAHshQUMaDTKcgBWrPlIzV8q5zkzSJ4rwVnbtENpq_n2MHLqsxz1aO_Ig4Gg2jQOez3RZBpN36XQlQFEm32RtS4BawuhunrnWkV-L7BGN6o5bdHGCR93A_5R5VOsS1DCJGdzQB7uiwuOrYBJPeZjqD1xe33wPkM3oCyEwF_aJ8=w1611-h93-no)

## 컨테이너의 포트 상태 확인(docker port)
구동 중인 컨테이너에서 실행 중인 프로세스의 전송 포트 매핑을 확인
`docker port <container>`
![](https://lh3.googleusercontent.com/k9qJOXNbi80ydiBUkJSyHzQCh9RPKzCrXtgG_hLGfgDiUf1bfaR7bPSOtnch42wWpXxGML6DJsLfYxxwguIkNCnbyP1sySHFkOKR5nl_2rg64sNQp38VYEeR1Rmn3UGl70zoACQnbns_6mnXr7v8A-8Rmge_KsihlQBjlRyU360UT7y-lmCBDqvPO4d2ou8QazCEjZX7Tzo1MLX92HcxI8FGrQNBofvlTd1TS39sPU-N843hysUIKHQxFeANS0zs4ayM_DoKIbN-_t1mSuAuUuAPYZJRPozQPvc6O70BaG7mynJbypLTgaw9BdOcQhJOnsjM_5fN349I41org1l-HZ_Zwe8MXItJD6lJ80iXH34qN4ZqxmAuivkxL4vQMbe5tsTGPrLk-sQFYKZiMfaYxkUHZ6W1OfCZJZPyRHhqJdVt_iXbObN-kBiRktXZpjikORrlZD-beUKDR9-OmsgdKtL7-rMrKG1ZVeip7dy63gFriplcosE7vOaPbMvTrtmH29cC-UE7u8A9GniOXXVkJsPG18FACRckJfpqsZAnNnq3rGEqisDsHVi67NjLJEEuP-ZErS2lhmb3Q-jnQLB5VHEXZWc06f8fuoHbce37tGCeJmpS3xmEl9VZkUN4vRPGWWvIwEIoYpaPnzMor8b4HQV0r9oSWDw3oLjh5_SSTOr3T4IcpCpEjCKBUU0LLX77bGLz9VA9PxgPlxOnclnT1A9X=w1308-h202-no)

## 컨테이너명 변경(docker rename)
컨테이너명 변경
`docker rename <old name> <new name>`

## 컨테이너 내에서 파일 복사(docker cp)
- Container to Host file transfer
`docker cp <container>:/container path /Host path`
- Host to Container file transfer
`docker cp /Host path <container>:/container path`

![](https://lh3.googleusercontent.com/2PQlDc-qfCMR5bc_y2Ugrcy4XUKoPRlQbV8PakqXCMN4cAezs_ImSg_KuAyMk9osJylxsErWqpJOMgVQW-KG4pmZ6B53MMIWoNB1vrfyCKzUqwSZOzgidw283up_UtM_yzFdNh537_CdPjEwBOJVVyyRnwLKdrnR3ujDNR9lpUloCsBaySVN4-KtPi0pRbhq3aTv9AuDkC6aJArJDTc-svpkCpt6u3LdKiPt4i8Dzmv08q16fPbyYTjlTDCCIXzpvhVlkpSRN9Y5c3ec5uVeodRxFBDSHkG9CzSaFJ_J3MsrCIDGfB8foK1I5k43WGPejWmFTzOsShC3ZU2NXZ5txWG6PApfQy57uAlGa7f46YlTOqTRQC3L4gZZIxQoPRGaBhe1EeqvP3VEND6ZJOHG1i4j7O9KDMeIYq03fvr6ZHCNXX6LowauTTgMMqtw60IlQABga3sKX8Xu-0ZY2GXQ5K-rmkyyDAuR-_opNx1ep9jcMAAbOARAOBwCd6DpYBNHlbUOYzrXafH-nn_wUxJBMjdGuSwSX5bohXsKoHygHDuYlMZ7gCbeYnn3TCj9xhd2pyireRBKde_JPG36adv8hHa1g8lvmwf8xUZA7Yabz2YprN0q-jgKJM23BhRaHeuVjD-BAZ7LhDp9QzOD3stWk8Cj9kGpJCgE-evraMPrAOPSA39ZMnJniJpVmxdPcvqX1n9dpTjp2mzE4pT7uVS6pq7-=w672-h387-no)

## 컨테이너 내에서 파일 변경 이력 확인(docker diff)
`docker diff <container | ID>`

[**변경 이력 구분**]
| section | description |
|--|--|
| A | add a file |
| D | delete a file |
| C | change a file |

ex) 사용자를 추가후 `docker diff`로 변경 이력 확인

![](https://lh3.googleusercontent.com/GLvxgag99ICeSA0o62lpfERc90uAnq4b8J39Y3_sA389X8QLd8dmriT2K3DsjdWAVJz4pgxlIeS8jk0ymuqORQ0lXzK98zHV2fON5GkeI0lrxwXzplyNwgX6sljc54kUC07tOG6L3qtr64X3SgkhHfzs46Dyrz_KeKp9HirprECjxPAepzYMjAYqQDR1F6m2xlFkmPg7WaXVlPeHUcknaF5B9h87gFz6rrAECPyPryqvbEb1zK8jRGt9cxK107ERdAIHBiIAx0y0nYvE3hMyZ5-SCp1jAn87z1wgp8RAwzfb7RuTL-c4do91DADinx_s-NlhJjsG72PIMfYtfHioKjtrPA4MjZiVdpYhF2BV7736PwSpEHsEKdDpwwvjfryQEInkH-dlv-l3I908YO_0m6JRAzLMFDZvr3to2R6W2tdIqPs4IHpz6QElkxnFRVL_22WxB-h7xZmBs_vcK5e5OJqkntE2vFNpHg5prAIE8DRS07Fbi4kAgc-g9pEEjRN9VtDTRA60Nq95z97rqRPduZ0fufgL1cMqCg60htU8Zh80pSApIKtnHBr9-Q9kG8xc_WMA74aU-4FFVDDxGJ5YqKBtT248C0lIM1mL3wDT3ToLbRwabJtct2NrYkq9jEgklQr6f7VjfwjGQhheWvbvxuOw15rTUigo7Nc3o7qmn34kKIDAdjZXri9YjdA8WWw--AenSHcjzShy3LAFz71mt59Z=w417-h524-no)

# Docker 정보 확인
## Docker 버전 확인(docker version)
docker의 client와 sever의 버전을 확인하고 Go언어, OS, Architecture을 확인가능하다.
## Docker 실행 환경(docker info)
![](https://lh3.googleusercontent.com/AnrCBaS8wMP1IXUPqtzikjg3kcUxgGswCz0vdcAwy6r7uHvITRDkNt9OMcK9eGIBl_FDfA4KYNBgOSazjzt5idiSEqPfdFbabp2UCrZwm-8I52SNdhmvoIBC6QlUyLhHTgnOU21YsZznPybZFTcj4h89xOTM_AG1T_kI_LSy5M_EjvR-MI1Il_JCjcNkvWPcy_bunMUnHEYbQqeQQJDOAkBBxocsudHWCKHp7VcGjIMNCO_DolcQZjnSc3PJrDcRxm91nYbiK5mdHfuPF9hWS03NY0FD9dmlP3DAfp8jZcqeGH_wazIvsJXTo_nmAb5W9C4BLuOIuAFyzAX2QTKrPRKmr22dK0pGz2p16f7I9-89Zuku4HVHOFx4TKOeEwXeT3gX9D5D0iPVOrqz_QnjoMlgjuGMsdXCLxsNOLYaWY4sBQwiJhitVWACzDGwpZRUYUBhyDBI7IuYKJHLEbwrtf9qF5sf2IBz5Dvz3Lip-sngommXssg10e-fNWlS8Mlg7KpW_srwPvxEyMaw63HAzBy-uo5NdSUEd_tv_dfRLxtZaAjpbDt-rKxurOPMGAfd-bwF4oRxEuRJkRIwPZj0yngEb5Dmwww-uCwcRvEe6mi1279_bGZnEEQuLYQWr4BUki0wrshRWAo6Y5rlq6VoP9qOSkg7NRXOnqAYO5VrgErnmPuNsOwvmo5JZbde3BiEPu_MZW3MjlrYE8Cu1Npyn4Pa=w849-h943-no)

# 컨테이너에서의 이미지
## 컨테이너에서 이미지 생성(docker commit)
현재 컨테이너의 상태를 이미지로 생성한다.
`docker commit [option] <container | ID> [image:tag]`
| options | description |
|--|--|
| -a, --author="~" | 생성자(ex. iamJjanga <namils147@gmail.com>) |
| -m, --message="~" | 메시지 |
| -p, --pause=true | 컨테이너를 일시 중지한 후 commit |

ex)
`docker commit -a "iamjjanga" nginx1 iamjjanga/webfront:1.0`을 한 후
`docker inspect iamjjanga/webfront:1.0`을 통해 author을 확인할 수 있다.

## Container->.tar 파일로 저장(docker export)
구동 중인 컨테이너의 directory 및 file들을 모아 tar 파일로 내보낼 수 있다.
`docker export [container | ID] > [내보내는 파일명]`
![](https://lh3.googleusercontent.com/JCZOCElAEAb9XfqVHUqLGi0yAGNMD-Ido0HBxpFNzOnwFks_4aKq5Z2shhkag7TJ4L9yi3uTw9e1XoxCLEJt6qAk_6GiZIdsx8FceQfnaP88HZvRNbDNi9XwLgtrwxTZdHcmMSR5NHuExrEEHnqVmTQ1Bkdgv8ytPXIIHS5HUTGcAvpJTgLq6ZOGeidjk_ai9KchNEUzYSzoxSuGNuEwmdG5TtKwl-8xar1CStH3XKnlS1PS52F7b0yyHjEFcqcMREh2cCDJqtB20sQBRhY_eLMmsntCHUq3mL4o6Hi2q4v1wOKZ7jF4_AZ1EPHdiCyaqShcCttZ5VlCXDeuLVzvjzKJyLxGDtX_oJ3XeYzgK3akruGPKbHzrVSL_y778QEwc7gE86sLNZXa8w5E7V5mNVzFZC6bBZb3e6DrXtQk8BSFXXbcFG3PVa38rc99NiHyASO-ZvvTKsLo1pM4aWRSbw3qB7ZcuCsJEl_SgoEMpKAJvtd2CFyfIhsFr231XkjY1PtdbAyIsvshiU_IUjVgo2UH3infjbrkoijZRzf-oYhZQG9TnF-BJJZ88nJISIfGxssVVX5FuENPOzLkv4Vf4pU29Pq3Vu7sckYg7_bPU9jD3oyc-JGnEfm85nbzA2cMY2zDMmlDzI-s65gkY3jlD5VKQfnztv3NTnj-NosQDF2eCP-TMAtZlmqkX63Xm4qfC2SXHnMQF_EMzxucXm6VJ_xV=w1328-h454-no)
nginx1 container을 nginx_tar_test.tar 형식으로 보내고 tar을 압축해제 해보았다.

## .tar 파일->image (docker import)
`docker import <file|url > - [image:tag]`
- import문에 지정가능한 파일은 1개이다. 따라서 tar 커맨드 등으로 한데 모아야한다.
> **지정할 수 있는 파일 형식**
> - tar
> - tar.gz
> - tgz
> - bzip
> - tar.xz
> - txz

ex)
아까 tar형식으로 exported된 container을 다른 이름으로 다시 import해보았다.
`cat <tar filename> | docker import - <new image name:tag>`
![](https://lh3.googleusercontent.com/FzEUjh1YnccW3PJEcui0T6_JLRy3E6k_GP8jDU9EjINP6Mvu0lC861_Ihz46a7TMyg7P52YtGmNOYuOfzfRW06sP717Zb0LBz4TF4znqZSHegc9TgDr6z9qvWTcoU4deAoTAt3X6DYgvS5GAwuBVXF5WkowP7rbz3surT6_MhiNvtE9A-ksguakRJQ1r01dvXOOIlr-lTdSuNNLN0lu6pgc7_5GHP-YwyOfm9vtjVsjI6ZWpUv1mT0k268kJF5iTa7aD5_BCBegI1aP68xL25qPyCDWK5wftnH81K9qUW-xr1nUhtk-QfcJ6EqnvcBkmNhpie_qzzkOdE9EvYGA7dmBF9aoIE1iE98s8qVxb4CGPukBjj5OynoTukGrtYwyKAMxPk-NSx5cGlJq9lwMdU0FqzJWPtsLjYh_7Oo406q-clb5UW2qyvGhkNBQChtc_Yz3KSNdOtdO_Rnuz2RZFkEoUpjPvtDfMkSqakvGi-TeI9Kq5yoF8bqzCMbZO_CXbefejxFhDBIOB12_f4POCXbuLLLHe2bcJz_KJSD_ahF2u-wX_P98ew2U4v3Lib9keKCWOffWdIlSWSwILT8hdKoXvjUHX7UW9Z915sVHUc4jys4g1sl3P7XdqpVb0_lSascUuXK4VOuVVENmkdEm5uSUsrRz1XxDo_h6ApAXGYNRJuGLELdHrUy3EGDz3jBcg0DcbUDrwNOkvpxa8I-7xklSY=w937-h315-no)

## 이미지 저장 image -> tar (docker save)
`docker save [option] <filename> [image]`
| options | description |
|--|--|
| -o | 파일명 지정 |

ex) `docker save -o export.tar iamjjanga/webfront:1.0`
![](https://lh3.googleusercontent.com/_KTZd_tdXmzwMOMDMGkPG2mds023_gM4rUU3CxwCb0rLWhuQcmoPGTHyIXZIDQpZR0VNtxfPfE8cNXb5XS1LT1sr-MYe0Sa5OiavqcMjYfYeQJoBPaYcIEZyUkDzMaZRNx18S_5rtgORnhxSBTiPv4EQGfMbBJycyWMBxan-uX0kneCfaSyi4DEu6FVu49RN6d316CxDAk7h9wbgA3jK8ht9eV66w-soRLBwZCa6Aq0NKbNE8aV1XmvhF08hPiu3xp2U7XeuhuosygXhGx5w6l9bFyI0rdlu_7EtJtPQpc0PGQ3HD1L0YHIhWpDPDQJtafrlBMpSLQX77Tn7Vpi4Nj_I0yJ1GhLlh02ED_D9fYlHnZqjjJ-LH26bXbfa3ufGAjvecO6CrMKb8tNgOMBNSCN-_o536Nbvu5wSbeUjYtVIxcu10I8v1GpV9bfn7fTAkj136TRQrByDwcVWC34Uc07-QIOMQUUKuUXvTMecEG7QAw-m_P1Dcf19JCRJ7vLzP1TR9u4Vdx63iwT6YPmf6B24XrHwNkYorZlADNorUEGhr4qg0SUpbWtl692kBVUpvrh16OZ_Quz1WlW2ZqfV4VoSqGazZPMz2LDnQW-wRBlAICuhK5p6DbacuzLJ-ftW4hRq10ChYx9_pELTsVXtBiL9nuR_Nxgx_TG1xDc053Dr-gyF6glhrc4J4AVuQLVMTvz9kFeZHZKamqzTV1yrHMZC=w896-h314-no)

## 이미지로 되돌리기 tar->image (docker load)
docker save로 저장한 tar 파일을 이미지로 되돌릴 수 있다.
`docker load [option] <filename>`
| options | description |
|--|--|
| -i | 파일명 지정 |

ex)`docker load -i export.tar`

# Dockerfile
## Dockerfile 기본
### 1. 용도
Docker의 이미지를 기반으로 Container를 생성할때 아래의 정보와 함께 인프라 설계서와 매개변수 시트 등 정보를 별도로 작성해 둘 수 있음.
=> 즉, Docker상에서 **동작하는 컨테이너 구성 정보**를 저장한 파일.
- 기반이 되는 Docker 이미지
- Container 안에서 실행되는 커맨드
- 환경변수 등의 설정
- Container 안에서 이루어지는 데몬 실행

Docker은 `docker build` 커맨드를 통해 Dockerfile에 저장된 구성 정보를 기반으로 Docker 이미지를 생성

### 2. 기본 구성
- editor로 작성 및 변경 가능
- 확장자 불필요
- 명령어는 대문자로 통일

| Command | Descriptions | Command | Descriptions |
|--|--|--|--|
| FROM | 베이스 이미지 지정 | ADD | 파일 및 디렉터리 추가
| MAINTAINER | Dockerfile 생성자 | COPY | 파일 복사
| RUN | 커맨드 실행 | VOLUME | 볼륨 마운트 |
| CMD | 데몬 실행 | ENTRYPOINT | 데몬 실행 |
| LABEL | 라벨 설정 | USER | 사용자 설정 |
| EXPOSE | 포트 export | WORKDIR | 작업디렉터리 지정 |
| ENV | 환경변수 설정 | ONBUILD | bulid 완료 후 실행될 명령어 |

Dockerfile Comment

	# comment
	명령어값

	# comment
	명령어 값 \# comment

### 3. Dockerfile 작성
#### 베이스 이미지는 필수 항목이다

**FROM**
>  `FROM [image]`
>  `FROM [image]:[tag]`
>  `FROM [image]@[Digest]`


	# 베이스 이미지 설정
	FROM centos:centos7

#### 특정 이미지 설정은 digest(유니크한 식별자)

digest 확인
![](https://lh3.googleusercontent.com/zInyfitsMwYiBQePKdVnOt3fllsXH1SO2_GdtByh480Mcs8TMbbNOR53kcp9YQzqHOb8Z_UxhwWZ2A_zY_1BgyWrhyh_BDfHxjZ8fiPgKQ9GvHhLAmP6Dlxf3ejPfmWDdvRm6esj8dWfiPA3te3dlwM75627RN6s9TH33Cd5VULd9K5fX7WDjj-grG3-KoQjyTMsxdVDx9l6adpVy2UanMnMuikHD1-JUhozQsM44R_hxEH0Y_0UxzBhZ8HJwavwcV0qEO4STxcASNc1-fIfj8zaIttykhTKjQJtykAElJU1xF9xMgvHFod0h3E37PlmZ09HkWJBV51c5JMaGLlOw6DpGZGbrBBUSommmM73KANmn5SjoOEe9mEQwAOy5qaQGe0RFqGZ_-Bk83WM_nbaoIAyPNciC4HMdHj4Ewb8VYFbE0M4R8NIEra70rrDiRPE64mqUpAok_g82czJTp-DKnuPW89ia4d0vTzkHRmQEH-hlSYkY8DqSPcCVzjdqtDXi-1eZY9GByHoHBDl2Gh-p-HRMWGxpSzz4tjrILlT_TMX4zF2TN0W50MdTis5MmFinjIAn4IGh8na1_plgt_u19MErIZ3ZHSTK2HqADDjSXYG1CQGu_rwiryo6K-WS3DXjBdhsJfsjwAW63VUC_Sdyczlu2-7P4OKDgT0T5DDVJPh_QLEJnm3yg7Fc6pUMQ2X7GdjlXV0-HlLSoPpT_PxRy5B=w1620-h205-no)

digest를 입력한 Dockerfile

	# 베이스 이미지 설정
	FROM iamjjanga/webfront@sha256:bb63f4b05addf9123fcf72e09ce5df83acb44f2e26598b17b809227536f67274

#### Dockerfile 작성자 저장은 MAINTAINER

	# 베이스 이미지 설정
	FROM centos:latest
	 
	# 작성자 정보
	MAINTAINER iamJjanga namils147@gmail.com

### 4. Dockerfile로 Docker 이미지 생성
`docker build -t [image name]:[tag] [Dockerfile path]`

#### Dockerfile 준비
Docker-machine 기본 환경에서

	$ mkdir sample && cd $_
	$ touch Dockerfile
	$ ls
	Dockerfile
	Dockerfile이 생성 되었으면

#### Dockerfile 내용
	# Set base image
	FROM centos:latest

#### `docker build` command 실행 및 이미지 생성 확인
![](https://lh3.googleusercontent.com/flcGXmYrWI8wf5fZFNhyZ-sxsBFpb9lDqzSFggTM6Nk3kav-IFGQwiaEHZXb2OpLyMVPT61H2kPfZQ_c3G8mF_BghIW-iTRkyu8xC6f2n_Gc-agG-ip_FovdVEdJBk-2o-IrjcigGc85jCDTs30bEw8HkzwHyGCglqwNSLCbvEKBr6EuKI8qgaOzvxUoboDR_dKx8vo0TIX1x6Jp0DNHYokBOeS07SQ_yj7Dicu_DJEkiIE2UFbcKvbteBnYyCP2EVKX43A9SWUSfl1AAoiORL9Vvn1sT84TuirI5pEojkspXlSWFD992v-LoJLXYg6yoT6U2YawDWee2Jo7CEe_7oDMiwgBzZaD4xO2IsE9FLfgRYLvvrfSCinxmVrkjAKFACOUCgpK88PD8JYFI1uwKg-k39QLJ-DAgUNQvVy9h8hRNwj1Ux8bsqOlm93E1R9Fa-ashfo2UqZEcuhvBac1ULgEwN-YA5YRjHdQk_1ChmNpnX2FtRmnt63bWyUCfZHOxFk3-O2D9xFVY_n1w3pK4SVGTXtJPAEVKfU99YNeoaPP_Qsh3NA7aLYgevi3ZlAKPKovKdC5vyS3m_yMY8MjNwuRwa3-xKjfICupflGEsvmgnPJPVwO-48S7Zy6a2N9dQDlC_dxtEKuKuG6olsuowyQigiaqRLuARIx-qtiTtUrSMJrGWXSkSsKm8k-awUotEqHRghLlHiCTjBdJsayrJsMy=w889-h364-no)
위와 같이 많은 base images인 centos가 있는 환경에서 tag가 다른 복사본을 만들고자 할경우에도 사용할 수 있다.

#### file명을 지정하여 `docker build` 커맨드 실행
`docker build -t sample -f Dockerfile.base`

#### 표준 입력으로 bulid
`docker build - < docker.tar.gz`
단, 표준 입력시 Dockerfile 내의 ADD 명령어로 이미지 내의 파일을 추가 할 수 없다.
하지만, **tar 압축**을 통해 **필수적인 파일을 표준 입력으로 지정**가능하다.
![](https://lh3.googleusercontent.com/P0AlMdsAjr5f14VO1Odtk6GtjGTOlR8q3a01ltaKiSFXfzCpLPsxuHN78eSc6avbwaU52rd22Te7dNLXyq_uFn2iAP3u3iPXZIIfZi8revkdlIRm_SX_QNgxdMLUiEekUiSVBioGv0VVcWR1HyrEjz7P8S3nz63dRo83nxdZgyH-kHMZNVhf8CxX6lPhzzHB1hAn_u6_ORQuaRfyIhD5yL8x3910krii_kwZO4NezuRXlXlCwcvkYLC4EDUxq8rzKwUw0923JyRV_8IcApTiFsLvAKT-1CRLpQgUblwFNZBpPa8RpPSDT8upgOakOwq1xsLC2j5Pc5fRnUFA8InNFLoDDSumPPXT06G2sDVolHWJHSV0xr_0Z_rM4S5osjgu6aaUwy_JhmVDIGjLk2DoUXbQargyruvQp34oEIAXnKaa4ALAf6kxxSocO6NBRGLks0utnQzwhjHXZM0Jsim1e5B6cepY3I6eFtNsD-F48Ab6F8pY38gbqGFGuNLSn66bUEtq_JAHxgJ6bTTK111P1BHUgBiIlcD_S6tZVitncwQZEvgN42_kCUiB4xpsMnZW5XlBQln9wXrdfno7y8RMkZwEeqHRN6dltqCC36CJ30zM3LZ9jE4if-xHLcflWQMuxMrsK4SGoNQZgD34LA-M9u_0HLwPVNx2y4L566SOtknbkDIJbyMZWhVKFlTB0ecEj_eHJV7Y2asiE3hEb8KZbaQc=w983-h477-no)


> **build에 불필요한 파일을 build에서 제외시키는 방법**
> 
> . dockerignore에 제외할 파일명을 입력한다. 여러 파일을 제외해야할때는 개행한다.
> dockerfile에 ADD옵션을 통해 dummyfile을 추가해도 무시하고 build한다.
> ![](https://lh3.googleusercontent.com/SRNJtW3wHFX-83u8Om_rqpqDmnxrGaeuiq2SG_HzI9M0kfGrTYR4ldxkBgtCnCOWoV_lX3gw9JeDryI4ApD_W4ls_l3quo1ZR60EVl03wlZMUR0_4UMXF_vF6SLv1EEDtKSRfTKDuLhCW7bleVXZ_ACsWuJ9t3lE8PSMZ4a989f0HgnUEvfjXtiyxGD5O3OZKiUqdJjbYQgM2MXo2RdbWfShSX_RhPzbI3iFMMkmImF6pybbcXiLGy4B-E1Av_FCfXE1xV272wWLPDL0GD6WpRMVv5q4hK-rPmO3DBYcs66sbXf6GfrfQbL4VOKTF78oSYW4m4VJ8PmDjy5OkIsMMmpH_3jslJ3D2fMufHE6JKOUa58PmtuFe8TR_lyMlIikFsEVGhuSkYoCL8nT8cplznzWcW4cprFqfnLZMr_CKb21lFJrRtxgcpYjzCGyQgDxcUYOVWzEzip5TaujWT0p__h-f_Qy7i12qvZ0G-iHXScdGGGUZiHwMCtor1rBRH8Rc3XakeznSML5sXUGS745AFOkZHp4YI42iw0c8x1GHj4i3TmCqyljl_VA6rCpOh4htlpgUbbw4Ziq4XdLeWwwDU9m-ZbUSHh_Erxf4qZQYNezsxzURzE0hj8WgkWs8UOB4d5qAWeyoilIzujUcutC6EgblmxauBmspAVQAvaLrPoAafHoEJpK4iWvevoiHGFSXZ32ilkn5fuqKLqb1JMpgT5E=w1504-h443-no)
#### Docker image layer
![](https://subicura.com/assets/article_images/2017-01-19-docker-guide-for-beginners-1/image-layer.png)

	$ docker buld -t sample .
	Sending build context to Docker daemon 3.072 KB
	Step 0 : FROM centos:latest
	---> 7322fbe74aa5 (**첫번째 이미지 생성**)
	Step 1 : RUN yum install -t httpd
	---> Using cache
	---> 81cc6bc75003 (**두번째 이미지 생성**)
	Step 2 : COPY index.html /var/www/html/
	---> c3a5957b31d1 (**세번째 이미지 생성**)
	Step 3 : CMD /usr/sbin/httpd _D FOREGROUND
	---> Running in 35a701cafd88
	---> 0bbd386e032c (**네번째 이미지 생성**)
	Successfully built 0bbd386e032c

## Dockerfile 커맨드 및 데몬 실행
### 커맨드 실행(RUN)
#### Shell 형식으로 실행
	#httpd 설치
	RUN yum -y install httpd

#### Exec 형식으로 실행
- Shell을 거치지 않고 실행, $ HOME과 같은 환경변수 입력 X
- 커맨드를 JSON 형식으로 입력  

		#httpd 설치
		RUN ["/bin/bash", "-c", "yum -y install httpd"]

문자열 입력은 '(single quote)를 사용

ex)

	# 베이스 이미지 설정
	FROM centos:latest

	# 작성자 정보
	MAINTAINER 0.1 your-name@your-domain.com

	# RUN 명령 실행
	RUN echo 안녕하세요 Shell형식입니다
	RUN ["echo", "안녕하세요 Exec형식입니다"]
	RUN ["/bin/bash", "-c", "echo '안녕하세요 Exec형식으로 bash를 사용해봅시다`"]

실행 log
![](https://lh3.googleusercontent.com/OPXee81uVXl7zmR5Z_DwfuJhMlM34g-XT9CYi6sZtaCpM31eWoxItglWV6tpOSFvOr_PpNNyRGfoF_-J1Cv3fbzFFJ-l3-yGKQaDJDPsj8YAhtX3II1HOEWb-TvDforP2hgWnotmcOKdeOCPq7mXRE4j3kP1prc10EkeCjsToZ_fCYuskJRANja28qY2LnkjdH1x6sRAFk8Hjcnkzd1WJsOqqPnQxvns2zanVv_ISsv9D6TrwYiCAh1CwQp3ZyHupgSCEfV8Ojeaqfn71n1ssGtAJqSr3X2IIfTEzsr8-w5hZixUkAuG5YeA0xBd_j-_WRDitp4NH_lJ2cWQqEcXo_JmjUZJMmLIe0eP-oO3WwZ1vZ3lW7yisZz845OHTDu3ouOh2Dbgy_Y7-C8DJHMc44WynAMjj2msEecCvk_-9ht4J9CGBL8JuGHDJ9wN3Dk6nZQiO0EAjRL_cGXYJsa8qZmxn-2PBFM_jIB4xudj0eCcBgS3ZyHvL5Z7fq-MulZROntLDX943o6Nkr_ubBlhkOnKvuDLfzc_X1TFdG5aX8cbIW-qwyZvByLfnqsptCJD-GKU1XQ_WPN7d2Ml7icYo6R6kf1EtzVUuPLC8i1sN7_cH4T1kTgOKgsi4hle_D3cjMhh0ldJNJOxU4x-VS6UjdCev1kDMfWCkwgSpfu5dhLnzY6BBePcX8Bisk18SARR22BTQLf4nXvESaiH1qSvU0qC=w520-h346-no)

#### 레이어 제한 에러
Dockerfile을 build할 때는 127해을 넘기면 에러 발생

	INFO[0107] Canoot create container with more than 127 parents
 
 따라서 RUN 명령어 수는 최대한 한행에 담을 수 있게 축약
> 4행으로 RUN 실행

	RUN yum -install httpd
	RUN yum -install php
	RUN yum -install php-mbstring
	RUN yum -install php-pear

> 1행으로 요약 RUN 실행

	RUN yum -install httpd php php-mbstring php-pear

> 개행을 통한 RUN 명령

	RUN yum -install\
				httpd\
				php\
				php-mbstring\
				php-pear

### 데몬 실행(CMD)
Dockerfile에는 한 개의 CMD 명령어만 입력하 수 있고 여러개 입력시 마지막만 적용

1. 웹 서버 구동하기 위하여 httpd 설치(RUN)
2. 컨테이너 내에 설치한 httpd를 데몬으로 상시 구동(CMD)

#### Shell 형식으로 실행
	# httpd 실행
	CMD /usr/sbin/httpd -D FOREGROUND

#### Exec 형식으로 실행
	# httpd 실행
	CMD ["/usr/sbin/httpd", "-D", "FOREGROUND"]

#### ENTRYPOINT 명령어의 매개변수로 실행
	# Set base image
	FROM centos:latest
	
	# Set maintainer
	MAINTAINER 0.1 yourname@your-domain.com

	# RUN command
	RUN ["yum", "install", "-y", "httpd"]

	# httpd execution
	CMD ["/usr/sbin/httpd", "-D", "FOREGROUND"]

![](https://lh3.googleusercontent.com/zP5u_N0cjv4TzHGIkudnq9Mj6YT-iT_WxQmyIqwr4VeKeQJU5iNW6ONiHuLZw6_i2N7vHEKIVVTiVDSnJmJZGaZXAPW3HFfopPZ-kSAWzcw5XY_DaleZq0Zp0-TI72L7_XN7Ml5uIbB-hpj9673zYz0ovq-RGv9Acg-KCe-HgcpJ6jp85PLzzAPsB1yD8oWc7Vma1tqMMQRaYWFLJ1WDmm4nOh85vqYeAWVXrKZzEeZb4-TsAtlZ4sSpaImQ8lYGnFVitQsohHZroXb4TEnNnblzn0lDqAVKqSCVcaGdMB7g44ptnhnh-JCBUumpUug9lkxkIGPaQL7QH0q0cEp-JHmuN259p3XEUTB7rXAPn7Qr8_VKw0PYiwk1Ym0z2D9tj-qOSsDLIaJiqu3RQ5khrZJ7qN0Elgp7hz8-xPxXjF5cokWf5XeJ8Q2Or3KoOiQonREnjyLPfIbLnOuoqvyc0kCDkr87VAPaZX_ub9AC2bJlZ8vaoGUs9E5fMoKIbxbj7zUkLWMR5U97Xu856OAWKe4R8M7CaZ2IVL-qNmxcrWD6etFHT_1qin-mMcsgtRU4qmZ081FmhvNfgBVXCGPX2xe3Ub_6Q-aOJaOhSW3cm7BvX5y10260DSR_xyoxo1yfuNdZ8mW8jjVTDJ9rTI_AN_x28sNCWPpxU0giJ7UYaDMO_vn5ZLBSm2DiUXTRxc3lvQFlex2Z6c06rHvM1Ojxp29l=w863-h381-no)

`$ docker run -d -p 80:80 sample2`를 통해 네트워크 설정
컨테이너 ip로 접속하여 구동을 확인한다.
![](https://lh3.googleusercontent.com/T89HOefNWJBWSA-WySjDIwhnAVFhL2VEeH2Bq5-Yi3G9xQ45HQzQUK-bLf2--SSJkE8Gj5YoI6SASiKJ9OAZ8E6EfQzek5MCXAcjIfpQAUvq1rsgOutF4j7OhcZmv-eZdv3LjghBKnpb_6yoJb9dij1NyA9tgJcJtLAGWhp_7XYI-fIJFbzFdWZ8qrhgRTxn_XVDM91JtrxQDktuICMzN8MP2MbUpzHQvNr9Jv5RYsnJeNbDltXTbACs5INikOZYm4bOsvznuEzn6ZC8F0BS1oBr_6kwC5Idbx73mTOTNQXPwz4LtqA9s3GPczkSW0QdJNdm-FPrR1GNYukr4BitVFRvEBNK5DAfIbQroRTiQS4Cb8mccUw3Zz5GBnstTWuterBNEM2YJDbRHt4runw5g8R8x1pixdX_hAOWrGf9nAAm5hAN0XfzEzLvELEOXxuDHMkE3LqJ1rsit0rPV7vYIJ1oaGfs105RV7kh879G-OOppH3zig2c_18bOLR2z9SetzdAKp5GOgHG7ZaO4p2L5idoXtk3ADw6aK09KcLnSwCdqnsKqE5yFu7DeIki_eStfTyATo-fZyzEOp6aGAvWRkxEPDD9dfaMVvTnIGLScF_JOjTnwYfHkkgjRHhBkfXGe8woDzT3DANQn1gOm_y13mOqAe8gRwqPa-eT_Jr65S4ebajLDQb_8iyslnDq_1MTs339yyTLpE9TYOYY7ApLgPNR=w1830-h943-no)

### 데몬 실행(ENTRYPOINT)
ENTRYPOINT 명령어에 입력한 커맨드는 `docker run` 커맨드가 동작할 때 실행된다.

**우선순위**
CMD 커맨드 < `**docker run**` 

#### Shell 형식으로 실행
	# httpd 실행
	ENTRYPOINT /usr/sbin/httpd -D FOREGROUND

#### Exec 형식으로 실행
	# httpd 실행
	ENTRYPOINT ["/usr/sbin/httpd", "-D", "FOREGROUND"]

---
# 추가내용 : ENTRYPOINT
#### ENTRYPOINT와 CMD?
- ENTRYPOINT와 CMD는 해당 컨테이너가 수행하게 될 실행 명령을 정의하는 선언문
- 즉, 컨테이너가 무슨 일을 하는지 결정하는 최종 단계를 정의하는 명령이다.

#### ENTRYPOINT와 CMD의 차이?
ENTRYPOINT와 CMD의 가장 큰 차이는  **컨테이너 시작시 실행 명령에 대한 Default 지정 여부**이다.

- ENTRYPOINT를 사용하여 컨테이너 수행 명령을 정의한 경우, 해당 컨테이너가 수행될 때 **반드시 ENTRYPOINT에서 지정한 명령이 수행**되도록 지정
- 반면, CMD의 경우 컨테이너를 실행할때 **인자값을 주게 되면** Dockerfile에 지정된 CMD값을 **대신하여 지정한 인자값으로 변경**하여 실행 

## **CMD로 컨테이너 생성**

	# Dockerfile
	FROM centos
	CMD ["/bin/df", "-h"]

위의 Dockerfile build해보면 `df -h` 명령을 한번 수행하고 종료하는 이미지를 생성

![](https://lh3.googleusercontent.com/JPcGbx2hLQzZKbLx3C3SUPESIeYGicnjhNrLgPfUYSR-1W6E9lbDgtYcivHu04_djgtcVwV4EcN_TRT-W-uwBDiQVmujXdXyXMDkD5WtSW889gvjWmfGAYlM-RP3dl6jVRwyuX_XOS6hVd5nDt3TARKNFsF1TKV1K5LJcp4H7iYYcVeyXY_zWWp9ESACFs0f9p4fg3nlsPBIYTwLamqmGcg-Ia68uaY96cVRcZ3-A-2C2ASscO3gbCcI50P-wDzsMUjogzMGPgYWeluUCpK_6YDEoXbMHagT-H8F7U3sqbAkUFg5xTyq2MzpJmPkI2y5FcfUzl87sXZM8MKuQR2l7lk7Y3mkx-oMCrsrXqKVar-Q5dS-bl9IWh8GxScw-AScHRrSpk2zXU6cyeBm-gvRfLWvR6vwPLtibRy2fePA2_C8mbCLCW6ilV1RfDWnfHUuDwtMeKR0qleove-qL0sd-_AOs54hHfpdfxadjx2hxc57pUXmzcagu_c-X3GosT8K-6wfPWKCv4wJLeb4e6_Hj5hoMkIvsDxtt4RN8ppzKsqG2kyPaoOlx26vuuGf6IP6py8ec2_6LiZJm2aepXQLMZXpSKWqPQTO7p2OhBe2lhCmGcoeIYTyu9ydrYYj3PfdIkUJK_FyyoJaWqULjaFV93lhupaGVzNArNMCsaxe1ETgN2ADuS10tJ4Qqn_eUJ8dr9Xexk7E7RZ7rqa07lJTpkf0=w952-h299-no)

생성한 iamjjanga/df 이미지를 컨테이너로 동작해보면 다음과 같은 실행화면을 얻을 수 있다.
![](https://lh3.googleusercontent.com/8muj2vSDkokfPCJPec6z1LPEc7AVb5bc01uYWvyaUrPPjM-SS_ft06eTATqxG7xGYx7HlbhASXTYR8-v8eGLofNXQE55aglbKP0hRppNa7uciEq_u3lyiP2z45fFG-q9hyZZDkdMtX8O53ppCe4wMjbcwK2ljFD1prLCiWceUDzPQlPCoSOkQn-nxG_O3Y4ShbMbx7LG250nQXTnU-Cd_VPnpFU3tmuCCrGrweM2ZHQg7obu3XSZCvEZUERjCML70iXlRE1aDRFbapCdRiH0VBXuTNJAk-sGF2R_z8BQLHJsjokiYKbN1S80wOR_uE9LMBC_elCjnlM-OV6VPHZfLvUx8lQ5ndWi9RkwHwCbyvCDdJF9FqSBhFGqM9Ywdzofm6ARw7BaQM1mFlvmt7nglcPNyyrRQdFKA6LrtRMeb_HWClOsT0xwgAo5cmOgVZwx7QbKaz3XPNab6271b7fJ4lKUwPAQtIW57RRMbUkuZJBLHOrCYwuFhoDUbz0kVn7hyxfs_spsCitBskEvzZFgWHzyd2RLrLaKcrRAH88Ujn48blkkYait5yDlnjthgSbRsolpigNh5z1n-CpFxDUH3H41zpEXv66fJpTCFK2ph0xtl7ugoGTkbYjM7VntmmDs7As2T9TatuDWX8qLnek0_EjYIHau3oKazcfSeHiLg_5-No5z24WoeLXXXG7rtY7weh_0m1d3JRiAl1kFqSyeqFKb=w952-h268-no)

이번엔 컨테이너 생성시 **추가 인자 값**을 주어서 변경된 컨테이너를 생성
`ps -aef`를 추가 인자로 주었다.
![](https://lh3.googleusercontent.com/sRZ8f3mpkwIyPJO5917FZUKmdVhvUzxz-6XJSPZlZnhEyaClG6DXN9LPAxAdFo-_bguzIPHN6JtL8dbcORM5133td0uTUqUuBZ5jqlhDcOxruW5PBHtQ-_T8JW4EjiaFgIoPbQ0C_qa4zSS8WvG00rCjbPecvpYwrFgxDjJHkA6i19f9WRr2h4M3GJvoRv8VX1xrenbploYG8iQpB290ATMeNeh362Ec8Pxc2nyRu4hntLmJx9u3maUqnRYKRcDzWIDRhWpFt_vT9Y9d7egg_0NpX3CcYHYgTsUb7k0tqxUhqI4mgGIbjwWz9Ft4PMytKbHiSeGYLWXww-5OhzREuIOzksWkmARMmu3bT5regtX3eRqBMtGB8K4XyWO2TMefNuZ92Mi1XouuceqhQoErdopJDB1PW3XMknG2JuWw5NNS49jLokG5WknPxM67l1ja-Wgr3oUYr4URMGgCKDsEG2JZhuA_h__FEkuUkUWvsKJjJWFqEo_vFUBeJjNTMZYjMLyAM7dkL3fofqkh6Adle_603zyAHiiWIm0GPr1ikQx1B2A5Ji_pQhFKAjBHOxgtuFw7zysBwRKZcadAgqxKBYrXPIEOgCResut50oU0hX_pgK588xUk2QAJ3_f68nfZSw8soruO-9a_BSRNdos2OyplFOouHWhv-GzraXdeJj2wawZ6EwZXcbAanQ5QVYFDyqw8dqM25A00yVd_1KEoxyyL=w952-h92-no)

`docker inspect`로 **추가 인자** 생성한 컨테이너와 그렇지 않은 컨테이너 비교
> iamjjanga-df **(df -h만)**

			"Cmd": [
                "/bin/df",
                "-h"
            ],

> iamjjanga-df **(ps -aef 추가)**

			 "Cmd": [
                "ps",
                "-aef"
            ],

위의 결과로 컨테이너 설정에 Cmd 값이 `docker run`의 인자 값으로 대체된 것으로 확인되었다.

## ENTRYPOINT로 컨테이너 생성

	# Dockerfile
	FROM centos
	ENTRYPOINT ["/bin/df", "-h"]

아래와 같이 ENTRYPOINT로 컨테이너를 생성
![](https://lh3.googleusercontent.com/YopHTSoCx1Uq5tGZ3mfJagJLi-eHISVQtJbSDXSzQ6mw3aMT3CY1fMRr9QPV9gh-1w5Aqdn-2YEUpAwTwFDziK1dJZcOfXasIc7bRBlqfOXM-GB6OSHSbNQX8Qli38O8xTEo93PxsuGU4Ay0d2m96ZVep9Xa_azpuJCmVskYOdtFpdYep3u5IsJojJeOP7dp-Bj55lFiOocWYawhtewMJyro6Kmywtol6lD1EcJW_jEF3_WQKzijOem7Cn16vBK2E5EmIGZ_etfgRg_xPObjGggQVIzdcskYDjli1xWylKRcajsdLXDKxf01i3vuSBTOOoJ92jZkxGV5r0myEEHu045yJcpoZwh9kZBGkR5TL76NIO02WQsjrADY9gCUWOAxr9ViqJ44mWdDGFwh5vgb00fZsCznsae3Rf0wT8PUO1MnOXAHpIRDsBJKr8oPQr_pdjLuqNzukwYqfMM6yWYObrbJh0xUR0Gw5nAqL-9dmUo0iEvaFoYIammIJVaQMQ51LRQ9Cl3so50cJhKsVDuAXszDM0lNUJjrF8FmNarQJ8QQotVJxEufx9rIuzY1vy0tL4-gG7tHESv4K9VpGnqCmIsK63DCBUx10cb2n6v7yrUw6ykC0Dxn1c7QgMrRx49uzAl-izvKbAfIMhWqYDIKeIC0u3i9xuFT-h24mRqx7aNeK2AuVt0PobxbTN6flOnuhYnqLyjclXqiyIEFXtkw0HlT=w952-h232-no)

`docker inspect`를 통해 Cmd와 Entrypoint를 확인해 보면

			"Cmd": null,			
            "Entrypoint": [
                "/bin/df",
                "-h"
            ],

Entrypoint에는 실행된 명령어의 정보가 있고 Cmd값에는 `null`으로 되어있음을 확인할 수 있다.

여기서 **추가 인자**에 대한 명령문을 통해 확연한 차이를 알 수 있다.

	$ docker run --name iamjjanga-df-entry2 iamjjanga/df:entry ps -aef
	/bin/df: invalid option -- 'e'
	Try '/bin/df --help' for more information.

유효하지 않은 option이라고 출력되는데 `inspect`를 통해 차이를 알아볼 수 있다.

			"Cmd": [
                "ps",
                "-aef"
            ],

            "Entrypoint": [
                "/bin/df",
                "-h"
            ],

즉, 컨테이너는 실행시 **ENTRYPOINT**에서의 `/bin/df` 명령을 유지하고, **추가 인자**를 **CMD**로 받아 처리하고 있다.

결국, 컨테이너는 시작시에 다음과 같은 명령어를 실행으로 에러를 일으켜 종료되었음을 확인할 수 있었다.

	$ df -h ps -aef 
	# ENTRYPOINT : df -h / CMD : ps -aef
	
	df: invalid option -- 'e'
	Try 'df --help' for more information.  
## 올바른 CMD와 ENTRYPOINT 사용하기

**첫째로는, 컨테이너가 수행될 때 변경되지 않을 실행 명령은 CMD보다는 ENTRYPOINT로 정의하는것이 좋다.**
대부분의 컨테이너는 생성시 실행 목적이 분명하다. WEB Server(nginx), App Server(node), DB(mysql) 등이 될 수 있다. 즉, 이미지를 만들때는 이러한 실행 목적이 분명하므로 nginx / node /mysql 같은 메인 프로세스가 될 명령의 경우에는 ENTRYPOINT로 정의하는 게 명확할 것이다.

**두번째로는, 메인 명령어가 실행시 default option 인자 값은 CMD로 정의해주는게 좋다.**
CMD는 ENTRYPOINT와 함께 사용시 추가 인자 값으로 활용된다. 따라서 메인 프로세스에 대한 default 옵션값을 CMD로 정의하는 것이 좋다.

**추가로 ENTRYPOINT와 CMD는 List Format(["args1", "args2", ...])으로 정의해 주는게 좋다.**
보통 ENTRYPOINT와 CMD 작성시 대부분 List 형태로 작성하지만 아래와 같이 일반적 Shell 형태도 작성 가능하다.

	#Dockerfile
		
	FROM ubuntu
	ADD loop.sh /usr/local/bin/loop.sh
	ENTRYPOINT /usr/local/bin/loop.sh 1    # Shell format

하지만, 이런 방식으로 이미지 빌드 후 컨테이너 실행하면 아래에서의 다른 방식으로 프로세스의 구동이 있게된다.

아래는 loop를 돌며 ps 명령을 통해 컨테이너의 프로세스를 확인하는 Shell script이다.

	#! /bin/bash

	INTERVAL=$1

	while true;
	do
	  ps x;
	  sleep $INTERVAL;
	done

이걸 사용하여 구동되는 실제 컨테이너 내부의 프로세스는 다음과 같다.
	$ docker run --name iamjjanga--loop iamjjanga/loop

	PID TTY STAT TIME COMMAND 
	1 ? Ss 0:00 /bin/sh -c /usr/local/bin/loop.sh 
	1 6 ? S 0:00 /bin/bash /usr/local/bin/loop.sh 
	1 43 ? R 0:00 ps x 
	....  

1. /bin/sh -c 실행 명령을 string으로 읽고
2. loop.sh를 /bin/bash 명령으로 실행

동일한 내용을 ENTRYPOINT List 포맷으로 실행하면?

	# Dockerfile

	FROM centos
	ADD loop.sh /usr/local/bin/loop.sh
	ENTRYPOINT ["/usr/local/bin/loop.sh"] # exec format
	CMD ["1"]

이미지 다시 빌드 후 `docker run`으로 실행해보면

	docker run --name jhsong-loop2 jhsong/loop:2 
	PID TTY STAT TIME COMMAND 
	1 ? Ss 0:00 /bin/bash /usr/local/bin/loop.sh 
	1 7 ? R 0:00 ps x 
	... 

**결론**=> 아까와는 다르게 `/bin/sh -c`를 거치지 않고 script를 실행
별다른 차이는 아니지만, sh를 거쳐 실행하는 것보다는 바로 script를 실행하는 것이 more clear

Docker 공식 Documentation에도 List 형태로 작성하는 것을 권장함

[CMD와 ENTRYPOINT 차이 출처]([https://bluese05.tistory.com/77](https://bluese05.tistory.com/77))

--------------------------------------------------------------------------------------

### build 완료 후 실행되는 명령 (ONBUILD)
Dockerfile로 생성한 이미지를 base image로 하여 다른 Dockerfile을 build 할 때, 실행할 커맨드를 입력하기 위해 **ONBUILD**를 사용한다.

ex) Web system을 구축할때,
- OS 설치 및 환경 설정, 웹 서버 및 각종 라이브러리 설치등 **인프라구축은 베이스 이미지**
- **ONBUILD** 명령을 이용한 심어진 프로그램을 **deploy**하는 명령(ADD나 COPY)


1. base image 설정

		# base image
		FROM centos:latest

		# maintainer
		MAINTAINER iamJjanga <namils147@gmail.com>

		# Apache httpd
		RUN ["yum", "install", "-y", "httpd"]

		# web contents
		ONBUILD ADD website.tar /var/www/html/

		# httpd execution
		CMD ["/usr/sbin/httpd", "-D", "FOREGROUND"]


	최신의 CentOS를 베이스 이미지로하여 웹 서버 실행 환경을 생성

	RUN 명령을 통해 `httpd`를 설치하고, CMD 명령을 통해 `httpd` 데몬을 실행한다.

	그리고 웹 컨텐츠(website.tar)를 `/var/www/html/`에 저장하는 커맨드를 ONBUILD한	다.


	>**주의할점은** website.tar로 하지 않을 시에 `/var/www/html/`에서의 권한 문제와 메인 화면은 `index.html`로 설정해야한다.


	파일명은 Dockerfile.base로 하고 다음 커맨드로 실행한다.

2. `docker build -t web-base -f Dockerfile.base .`로 빌드

		Sending build context to Docker daemon  2.048kB
		Step 1/5 : FROM centos:latest
		 ---> 9f38484d220f
		Step 2/5 : MAINTAINER iamJjanga <namils147@gmail.com>
		 ---> Running in 4deaafde5502
		Removing intermediate container 4deaafde5502
		 ---> e445f2f1cb9e
		Step 3/5 : RUN ["yum", "install", "-y", "httpd"]
		 ---> Running in 64a085094a85

3. 웹 서버용 이미지 생성


		# Docker image
		FROM web-base

	위의 Dockerfile을 build하면 ONBUILD에서 입력하였던 웹 콘텐츠가 이미지에 추가되어 build된다.

	`docker build -t web-image .` 실행

		Sending build context to Docker daemon  4.608kB
		Step 1/1 : FROM web-base
		# Executing 1 build trigger
		 ---> 77030b464eb8
		Successfully built 77030b464eb8
		Successfully tagged web-image:latest

4.  웹 서버용 컨테이너 구동

	`1단계`에서 웹 서버를 위한 인프라를 구축하고 `2단계`에서 웹 Contents를 deploy한 후 생성된 이미지를 기반으로 컨테이너를 구동한다.

	`docker run -d -p 80:80 web-image`

	아래는 index.html로 ONBUILD한 Contents를 생성하였다.
![](https://lh3.googleusercontent.com/x3aGgWjgEPsNebjQOJKAmwRp5xJ62bPnsz8tuijisurMf1HpeZ2GVf6Ko3MNuP9Ozh_vfPv_A0wF2Vgpm4KXEVZPpcha6XhVQhmWivDGf3ClOBwBGKMGj_5D0PdUNtGc9kp9M5X0Vz-RJzq3Lx1ze7f08oETUioWNgRvSI7uh_fM2iDisjoiUeMy1iNOX9zwNyx6kGoCQSGpizzwnIyre0F57hnXLZNKwoGYlXBsn8oiDypLhBNp5t0kwY5icjGyjSlaApQOFfDmSb7gyfTnQSKsH456FG3XZCFEMrGflMmVK2lh71pVGYXCfWhJJYvyXG4E4fN15k5oZ5rki5tbIJCdIXM6P-CfuFyl9jni9K-SUmgVmGlfSiO3m4GMmobZtOCEXHy1x4SJv239HE3PjwDwSLSQ7QYSunXLHYnCVwjS8eAIn5yfo9mzOMhu9lPPf7slsilhyIzmCinatnHAkOIa2HLO7gG4LqltFdQEByaLmKLoH6F2aTXTQjaE8aRuZ5O5aRhApuwe0sbEbBinaGEWZhWTkM8gQCxhAcvmfSdc84uGtqn8z3bK9iC8oUx95r3kaT4fCApU35_Ebp920H_TjxAilTxW7zhXoqznw61YG8FsbuqwF8De6iE65MlVFAF5VPB5jkss-7Z1Ou3qcWKJpLMdAClaNZrFadq-0-hHIDBP4Hz3KIZMNZSh-34AL2A8blebeZbb4SJpCk-FX-5a=w1440-h492-no)

=> ==ONBUILD==는 **인프라 생성**(인프라구축)과 애플리케이션 **deploy**에 관련된 것으로 나눌 수 있다.

`docker inspect`명령어로 이미지에 ONBUILD가 설정되어 있는지 여부를 확인할 수 있다.

	$ docker inspect --format="{{.Config.OnBuild}}" web-base
	[ADD web-sample.html /var/www/html/]

## 환경 및 네트워크 설정

### 환경변수 설정(ENV)
	ENV [key]=[value]
	ENV [key] [value]

1. [key] [value]형식
key다음 입력되는 문자열을 하나의 값으로 인식한다.

	| key name | value |
	|--|--|
	| myName | "SIHYEONG LEE" |
	| myOrder | Gin whisky Calvados |
	| myNicoName | iamJjanga |

		ENV myName "SIHYEONG LEE"
		ENV myOrder Gin whiky Calvados
		ENV myNickName iamJjanga

		생성되는 image는 3개
2. [key]=[value] 형식
한번에 여러 개의 값을 설정하고자 할 때 사용한다.

		ENV  myName="SIHYEONG LEE" \
			  myOrder=Gin \ Whisky \ Calvados \
			  myNickName=iamJjanga
			  
		=> 생성되는 image는 1개

ENV 명령으로 설정한 환경변수는 `docker run`커맨드의 `env`옵션을 사용하면 변경할 수 있다.

### 작업 디렉터리 설정(WORKDIR)

WORKDIR를 통해, Dockerfile에 저장된 다음 명령을 실행하기 위한 작업 디렉터리를 설정할 수 있다.

- RUN
- CMD
- ENTRYPOINT
- COPY
- ADD

WORKDIR 명령은 Dockerfile 내에서 여러번 사용이 가능하고 마지막 행을 기준으로 하는 상대 경로로 설정

	WORKDIR /first
	WORKDIR second
	WORKDIR third
	RUN ["pwd"]

	=> /first/second/third가 출력

환경변수를 사용한 WORKDIR 예

	ENV DIRPATH /first
	ENV DIRNAME second
	WORKDIR $DIRPATH/#DIRNAME
	RUN ["pwd"]

	=> /first/second 출력

### 사용자 설정(USER)
Dockerfile에서 다음 명령어를 실행할 사용자를 설정

- RUN
- CMD
- ENTRYPOINT

>USER 명령으로 실행전 미리 사용자를 추가해놓아야 한다.

	RUN ["useradd", "Lee"]
	RUN ["whoami"]
	USER Lee
	RUN ["whoami"]

실행결과
![](https://lh3.googleusercontent.com/j_pnSgF6OExL0y5WAeGaRYcEPTdbGgRx9qQdeHPeQHUlPjsEnrmVcIB_GiuMPEGv29bDNsG7WJCUcUr_C8NSRVLtdmnVDVTLf99J6pjyHk0M7Z0oa7aosBjEviHSI7DS8THoY5V6_bmR1fEDFK07i9DMV3U-3HildLEUQN3kUHJANMWT8xZFvsz0zMeDJL55BlTbN9NVjGk4bGQfV2IvkQ8cun-EERmEoySKd7zL1iP8NYMBMPQi678lQTOqSut52B_XrM5sHdtLo-RSa3a5kpTd_Hv2PJuq3wflIl2fCswy1Is-UAp6asLWsoUnD54O9j9nBVfOtifiyvRprpBJftnKCS8quhSZ0Iv-JlUClFG7TcIU5Epjzs53BZS_P4y56bFlkyX5O617_Qzu0AIIRe7wgEBzUmJZ8voGJBpttvd8ekDPEDwyX5xm6vlPFDulyJBiMo1LbqkjMxzS3MPEzpW1xyVTkF-Qr0s576WTxpTuLOawLkQliFznosIMtyf1MDfYLlxK9Jb0KVAUgYXT67606opEEcrwUk3eAK6rzPzNJHMdZleEMKztRBInj4F2QyKNBHfpitqRyS2d8-YkUE0Kj4M7QVAJnp17eFp8Me18b0ZBT3WqMS6tPHFvVKdfdhAGo1pOJwGt7XCxjX3nH6-U9uLWeBiA8qRShWEYebgpbGDgtiXR4OwmiDVLVKvs-_wwMfEeXOj2EmloL0Sa7Vxf=w666-h263-no)

### 라벨 설정(LABEL)
버전, 커맨 등의 정보를 담을 때 사용한다.

	LABEL title="WebAPServerImage"
	LABEL version="1.0"
	LABEL description="This image is WebApplicationServer \
	for JavaEE."

실행결과

	docker inspect --format="{{.Config.Labels}}" label-test
	map[org.label-schema.schema-version:1.0 org.label-schema.vendor:CentOS title:WebAPServerImage version:1.0 description:This image is WebApplicationServer for JavaEE. org.label-schema.build-date:20190305 org.label-schema.license:GPLv2 org.label-schema.name:CentOS Base Image]


### 포트 설정(EXPOSE)
컨테이너에 공개할 Port설정시 사용
EXPOSE명령을 통해 Docker에서 실행 중인 컨테이너가 listen하고 있는 네트워크 포트를 알 수 있고, 링크 기능을 통해 컨테이너와 내부를 연결

	EXPOSE 8080

## File System
### 파일 및 디렉터리 추가(ADD)
ADD 명령은 호스트 파일 및 디렉터리, 원격 파일을 Docker image 파일 시스템이 복사한다.

	ADD <host path> <Docker image path>
	
	ADD ["host path""Docker image path"]

ex)

	ADD host.html /docker_dir/

	=> HOST의 host.html 파일을 IMAGE의 /docker_dir/에 추가

WORKDIR 명령을 같이 사용한 예

	WORKDIR /docker_dir
	ADD host.html web/

	=> WORKDIR로 /docker_dir을 지정하고 /docker_dir의 web 디렉터리 하위에 host.html을 복사

이미지에 추가하고자 하는 파일이 **원격 파일 URL**인 경우, ==600==(사용자만 입력 및 출력 가능) 권한을 가진다.

만약 원격 파일이 **HTTP Last-Modified 헤더**(최종 수정 일시 타임스탬프)를 보유하고 있으면 추가된 파일의 ==mtime== 값으로 사용된다.

또한, ADD 명령은 인증을 지원하지 않기 때문에 원격 파일 다운로드 시 **인증이 필요한경우**, RUN 명령에서 wget 커맨드나 curl 명령을 사용하면 된다.

	ADD http://www.wings.msn.to/index.php /docker_dir/web/

>**tar 주의**
host에서 ADD한 압축 파일인 경우는 디렉터리로 풀리는 것에 반해
원격 URL에 다운로드한 resource에서의 tar은 압축이 풀리지 않는다.

### 파일 복사(COPY)
이미지에 Host의 파일이나 디렉터리를 복사할때 사용한다.
	
	COPY <host path> <Docker image path>
	COPY ["host path" "Docker image path"]

ADD 명령과 사용법이 비슷하지만,
- ADD 명령은 **원격 파일**을 다운로드하거나 파일 **압축풀기** 등이 가능
- COPY 명령은 호스트의 파일을 이미지 내의 파일 시스템에 **복사**하는 작업만 수행

### 볼륨 마운트(VOLUME)
이미지에 볼륨을 할당할 때 사용한다.

	VOLUME </path> -> 문자값
	VOLUME "/path" -> JSON 형식

아래의 예제에서는,
- 로그 데이터만 저장하고 있는 컨테이너(log-container)
- 웹 서버 기능을 가진 컨테이너(web-container)를 각각 생성
웹 서버 로그를 로그 데이터용 컨테이너에 보존하기 위한 순서로 Dockerfile 생성

1. 로그용 이미지 생성

		로그용 이미지 생성

		# Docker image
		FROM centos:latest

		# maintainer
		MAINTAINER 0.1 yourname@your-domain.com

		# volume
		RUN ["mkdir", "/var/log/httpd"]
		VOLUME /var/log/httpd

	다음으로는 Dockerfile을 통해 이미지를 build한다.
	
		$ docker build -t log-image .
		Sending build context to Docker daemon  2.048kB
		Step 1/4 : FROM centos:latest
		 ---> 9f38484d220f
		Step 2/4 : MAINTAINER 0.1 your-name@your-domain.com
		 ---> Running in dfc3c22ddda7
		Removing intermediate container dfc3c22ddda7
		 ---> dbe8df659016
		Step 3/4 : RUN ["mkdir", "/var/log/httpd"]
		 ---> Running in ed3be74b1770
		Removing intermediate container ed3be74b1770
		 ---> 6ff4ed85fa1c
		Step 4/4 : VOLUME /var/log/httpd
		 ---> Running in 1e4981459d92
		Removing intermediate container 1e4981459d92
		 ---> cad6c89aa52e
		Successfully built cad6c89aa52e
		Successfully tagged log-image:latest

2. 로그용 컨테이너 구동
앞 단계에서 생성한 로그용 이미지 'log-image'를 기반으로 컨테이너 구동

		$ docker run -it --name log-container log-image

3. 웹 서버용 이미지 생성

		웹 서버용 이미지 생성

		# Docker image
		FROM centos:latest

		# maintainer
		MAINTAINER 0.1 yourname@your-domain.com

		# Apache httpd
		RUN ["yum", "install", "-y", "httpd"]

		# Apache execution
		CMD ["/usr/sbin/httpd", "-D", "FOREGROUND"]

	웹 서버용 이미지 빌드

		$ docker run -t web-image .

4. 웹 서버용 컨테이너 구동
- 생성한 'web-image' 기반으로 컨테이너를 구동한다.
- `--name`옵션으로 컨테이너 이름을 'web-container`로 설정
- 포트는 80번
- log를 출력할 컨테이너를 `--volumes-from`옵션을 통해 설정

		웹 서버용 컨테이너 구동

		$ docker run -d --name web-container \
					-p 80:80 \
					--volumes-from log-container web-image

	모든 환경이 구축되고서 인터넷 브라우저를 통해 액세스한 후

5. 로그 확인
다음 커맨드를 통해 구동 중인 로그용 컨테이너에 액세스한다.

		로그용 컨테이너 액세스
		
		$ docker start -ia log-container
		[root@5a56449bc25a /]# ls -l /var/log/httpd
		total 8
		-rw-r--r-- 1 root root 1295 Jul 28 09:59 access_log
		-rw-r--r-- 1 root root 1018 Jul 28 09:59 error_log
		
		=> access_log와 error_log 파일 생성을 확인할 수 있다.
		
		[root@5a56449bc25a /]# cat /var/log/httpd/access_log
		172.30.1.54 - - [28/Jul/2019:09:59:12 +0000] "GET / HTTP/1.1" 403 4897 "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36"
		172.30.1.54 - - [28/Jul/2019:09:59:13 +0000] "GET /noindex/css/fonts/Bold/OpenSans-Bold.woff HTTP/1.1" 404 239 "http://172.30.1.6/noindex/css/open-sans.css" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36"
		172.30.1.54 - - [28/Jul/2019:09:59:13 +0000] "GET /noindex/css/fonts/Light/OpenSans-Light.woff HTTP/1.1" 404 241 "http://172.30.1.6/noindex/css/open-sans.css" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36"
		172.30.1.54 - - [28/Jul/2019:09:59:13 +0000] "GET /noindex/css/fonts/Bold/OpenSans-Bold.ttf HTTP/1.1" 404 238 "http://172.30.1.6/noindex/css/open-sans.css" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36"
		172.30.1.54 - - [28/Jul/2019:09:59:13 +0000] "GET /noindex/css/fonts/Light/OpenSans-Light.ttf HTTP/1.1" 404 240 "http://172.30.1.6/noindex/css/open-sans.css" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.142 Safari/537.36"

		=> chrome browser을 통해 접속했던 로그를 확인할 수 있다.



>**Note. 시간 동기화를 위한 Protocol NTP**
>![](http://upload.wikimedia.org/wikipedia/commons/thumb/c/c9/Network_Time_Protocol_servers_and_clients.svg/500px-Network_Time_Protocol_servers_and_clients.svg.png)
>[NTP 참조 URL](https://mindnet.tistory.com/entry/NTP)

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
eyJoaXN0b3J5IjpbLTEzMTU3MzcxNTddfQ==
-->