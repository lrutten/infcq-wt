build:
	docker build -t lrutten/wt-veelhoek .

run:
	docker run -d --name veelhoek -p 80:80 lrutten/wt-veelhoek

runbash:
	docker run -ti --name veelhoek lrutten/wt-veelhoek /bin/bash

bash:
	docker exec -ti veelhoek /bin/bash

stop:
	docker stop veelhoek

logs:
	docker logs veelhoek
ps:
	docker ps -a

login:
	docker login

push:
	docker push lrutten/wt-veelhoek

rm:
	docker rm veelhoek
