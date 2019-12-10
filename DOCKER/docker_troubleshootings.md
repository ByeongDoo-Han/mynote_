# 1
### failed to get D-Bus connection: Operation not permitted

Docker로 CentOS 이미지로 만든 컨테이너에서 `systemctl` 명령어를 사용하지 못하는 상황에서 나오는 오류이다.

이유인 즉슨, 구동 중인 컨테이너는 **non-privileged** 컨테이너로 systemctl은 CAP_SYS_ADMIN capability가 필요하나 Docker은 **non-privileged** 컨테이너에게 보완적인 문제로 부여하지 않았다.

해결법)

1. 아래와 같은 Dockerfile 생성

	```
	FROM centos
	MAINTAINER "Yourname" <youremail@address.com>
	ENV container docker
	RUN yum -y update; yum clean all
	RUN yum -y install systemd; yum clean all; \
	(cd /lib/systemd/system/sysinit.target.wants/; for i in *; do [ $i == systemd-tmpfiles-setup.service ] || rm -f $i; done); \
	rm -f /lib/systemd/system/multi-user.target.wants/*;\
	rm -f /etc/systemd/system/*.wants/*;\
	rm -f /lib/systemd/system/local-fs.target.wants/*; \
	rm -f /lib/systemd/system/sockets.target.wants/*udev*; \
	rm -f /lib/systemd/system/sockets.target.wants/*initctl*; \
	rm -f /lib/systemd/system/basic.target.wants/*;\
	rm -f /lib/systemd/system/anaconda.target.wants/*;
	VOLUME [ "/sys/fs/cgroup" ]
	CMD ["/usr/sbin/init"]
	```
2. `docker build --rm -t centos7-systemd .`로 빌드
3. 컨테이너를 구동한다.
	`docker run --privileged -it -e container=docker -v /sys/fs/cgroup:/sys/fs/cgroup centos7-systemd /usr/sbin/init`
4. 아래와 같은 화면이 나오면 권한 부여는 된것이다.
![](https://lh3.googleusercontent.com/0PURuIPy3ePp0bVD32VRtdvf8gwMwMqNpLHRp6RcBs2zWZhRQ90ah41_iX68FXAi0oTqrZF3wVO0s4BfafG7BeU7dTTbjqiKag8Wwvnv2RiqhOpEePcMYnaajubGw3izIqO2dQJA_3OM2e4cJCm0h0hQTyb6vdiqwiFZfHyk_KZ-pMkr2wRQIl2Nj84czO8sgvj7JhtkCzKFoVn8qmtqZsKQ44-rboIOQgzJHxQ_ZCTxFQegwI4tuTJ-qYD-S4Jf5pnkLcZqEWCQ7LqbPoUtJHGkYWH063p0XNzkYia96wQELIIN9amTOJYesCdrRy249NdW-AlW6V7LBmZe9Zdx6OpQKo_Gh277p5SKGAVxEA0oqXyHVEUmHPMxmat0vbVBIzyDkQhgR2mb5pJ_BcOjLT7vftmtQSMdwYLfH1EGGNvTf1Q4n2kS0J-FjwkkZwblB5b_jlDQCN87tIcfyELRzO7g1wmq7by8F_TYSCcpuZqekS0wytiagzIFZpoZto4lpKofqHqBVpZJ4bOFzn9fUIAJpKaHinq10E1mo5j_6VGdgmUDG_WenMDSwtbf72xn3uHQqfIIVMEDAfpBshYZUNV6GCrFGGVXbQEI2CEUv8Rvj-Sd61Jn2ed6woRCQwsncpDF6Y5y_PbZZvOs1mOhZy0YoaE609Wr_vKyoFGpqbL4e6GKCB1lff5ppaa-pjaqxkvFcFJkDUdFJUf02LO_Q9AL=w952-h725-no)

참고 github ([https://github.com/docker/compose/issues/4181](https://github.com/docker/compose/issues/4181))
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTEwMTk4NjMwODVdfQ==
-->