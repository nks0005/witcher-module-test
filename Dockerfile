FROM alpine:latest

RUN mkdir /work

COPY wclibs /work
COPY nodejs-dummy-main /work/nodejs-dummy-main
COPY Widash /work

COPY test.cpp /work
COPY test.sh /work

# 필요한 모듈 설치
RUN apk --update add nodejs npm make bash build-base autoconf automake

# tmux 테스팅
RUN apk --update add tmux

# MySQL 설치
RUN apk add mysql mysql-client


# ------------ widash 설치
WORKDIR /work
# RUN unzip Widash.zip
# RUN unzip nodejs-dummpy-main.zip

WORKDIR /work/Widash-a8508329c7bd10a9459232d4ccc80d6313c8b660
RUN autoreconf -i
RUN ./configure
RUN make

RUN cp ./src/dash /work/wicher-dash
# --------------------- wi-dash 빌드

# dummy httpreqr 빌드
WORKDIR /work
RUN gcc -o test test.cpp

# ------------------------

# todo -> 서버 실행 + 쉘 연결
# RUN unzip nodejs-dummy-main.zip
WORKDIR /work/nodejs-dummy-main
RUN npm i

# 쉘 연결
# RUN rm /bin/sh
RUN cp /work/wicher-dash /bin/sh
# docker cp bb140970962d:/work/wicher-dash .    # 호스트로 옮기고 싶다면...


# -------------- mysql 설치
# docker pull mysql
# docker run --name mysql-container -e MYSQL_ROOT_PASSWORD=password -d -p 3306:3306 mysql:latest
# docker exec -it mysql-container bash



CMD ["/bin/sh"]