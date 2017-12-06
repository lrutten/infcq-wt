FROM ubuntu
MAINTAINER Leo Rutten "leo.rutten@kuleuven.be"
RUN apt-get update
RUN apt-get install -y witty witty-dev witty-doc witty-dbg witty-examples
RUN apt-get install -y build-essential
RUN apt-get install -y cmake
RUN apt-get install -y libboost-all-dev
RUN mkdir /teken
COPY teken/ /teken
WORKDIR /teken
RUN cmake .
RUN make
CMD ["/teken/teken", "--http-address=0.0.0.0", "--http-port=80", "--docroot=/teken"]
EXPOSE 80

