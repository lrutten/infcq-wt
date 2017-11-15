build:
	docker build -t lrutten/wt-teken .

run:
	docker run -d --name teken -p 80:80 lrutten/wt-teken

runbash:
	docker run -ti --name teken lrutten/wt-teken /bin/bash

bash:
	docker exec -ti teken /bin/bash

stop:
	docker stop teken

logs:
	docker logs teken
ps:
	docker ps -a

login:
	docker login

push:
	docker push lrutten/wt-teken

rm:
	docker rm teken

# Beheer van de vbox machines

modules:
	modprobe vboxdrv
	modprobe vboxnetadp
	modprobe vboxnetflt

machine-ls:
	docker-machine ls

machine-create:
	docker-machine create --driver virtualbox machine1
	docker-machine create --driver virtualbox machine2
	#docker-machine create --driver virtualbox machine3

machine-stop:
	docker-machine stop machine1
	docker-machine stop machine2
	#docker-machine stop machine3

machine-rm:
	docker-machine rm -y machine1
	docker-machine rm -y machine2
	#docker-machine rm -y machine3

machine-env:
	docker-machine env machine1
	docker-machine env machine2
	#docker-machine env machine3

machine-ssh1:
	docker-machine ssh machine1

machine-ssh2:
	docker-machine ssh machine2

machine-ssh3:
	docker-machine ssh machine3

machine-ip:
	docker-machine ip machine1
	docker-machine ip machine2
	#docker-machine ip machine3

machine-scp:
	docker-machine scp Makefile.machine machine1:/home/docker/Makefile
	docker-machine scp Makefile.machine machine2:/home/docker/Makefile
	#docker-machine scp Makefile.machine machine3:/home/docker/Makefile


machine-install-make:
	docker-machine ssh machine1 tce-load -wi make.tcz
	docker-machine ssh machine2 tce-load -wi make.tcz
	#docker-machine ssh machine3 tce-load -wi make.tcz

machine-get-token:
	docker-machine scp machine1:/home/docker/token.txt .

machine-put-token:
	docker-machine scp token.txt machine2:/home/docker/
	#docker-machine scp token.txt machine3:/home/docker/


