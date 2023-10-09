# mysql 세팅

# -------------- mysql 설치
# docker pull mysql
# docker run --name mysql-container -e MYSQL_ROOT_PASSWORD=password -d -p 3306:3306 mysql:latest

# docker exec -it mysql-container bash

#PS C:\witcher-module-test> docker network create test_mysql
#PS C:\witcher-module-test> docker network connect test_mysql mysql-container
#PS C:\witcher-module-test> docker network connect test_mysql 9ef281fa1f9b


FROM mysql

RUN mkdir /sql
COPY test.sql /sql

CMD ["/bin/dash"]