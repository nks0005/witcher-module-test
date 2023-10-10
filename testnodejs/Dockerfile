FROM witcherfuzz/nodejs

RUN apt-get update && apt-get install -y nodejs npm make bash build-essential autoconf automake tmux

RUN mkdir /work
WORKDIR /work

COPY nodejs-dummy-main /work/nodejs-dummy-main
COPY Widash /work

#WORKDIR /work/Widash-a8508329c7bd10a9459232d4ccc80d6313c8b660
#RUN autoreconf -i
#RUN ./configure
#RUN make

WORKDIR /work/nodejs-dummy-main
# RUN cp ./src/dash /work/wicher-dash
# RUN cp /work/nodejs-dummy-main/wicher-dash /bin/sh

# WORKDIR /work/nodejs-dummy-main
# RUN npm i

RUN ["/bin/sh"]