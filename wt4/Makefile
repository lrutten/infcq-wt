build:
	docker build -t lrutten/wt-teken-actor .

run:
	docker run -d --name teken -p 80:80 lrutten/wt-teken-actor

runbash:
	docker run -ti --name teken lrutten/wt-teken-actor /bin/bash

bash:
	docker exec -ti teken /bin/bash

stop:
	docker stop teken

logs:
	docker logs teken
ps:
	docker ps

login:
	docker login

push:
	docker push lrutten/wt-teken-actor

rm:
	docker rm teken

# Beheer van de swarm

IP = 192.168.1.11

cycle: push service-rm service-create ps

swarm-init:
	docker swarm init --advertise-addr $(IP)|grep '\-\-token'|sed 's/.*--token //'|sed 's/ .*//' >token.txt

swarm-join:
	docker swarm join --token  `cat token.txt` $(IP):2377

swarm-leave:
	docker swarm leave --force

node-ls:
	docker node ls

service-create:
	docker service create --replicas 1 --publish 80:80 --name teken lrutten/wt-teken-actor

service-ls:
	docker service ls

service-inspect:
	docker service inspect --pretty teken

service-ps:
	docker service ps teken

service-scale:
	docker service scale teken=2

service-rm:
	docker service rm teken


