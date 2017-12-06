FROM ubuntu
MAINTAINER Leo Rutten "leo.rutten@kuleuven.be"
RUN apt-get update
RUN apt-get install -y witty witty-dev witty-doc witty-dbg witty-examples
RUN apt-get install -y build-essential
RUN apt-get install -y cmake
RUN mkdir /veelhoek
COPY veelhoek/ /veelhoek
WORKDIR /veelhoek
RUN cmake .
RUN make
CMD ["/veelhoek/veelhoek", "--http-address=0.0.0.0", "--http-port=80", "--docroot=/veelhoek"]
EXPOSE 80

