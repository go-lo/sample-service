default: service

.PHONY: clean
clean:
	rm service

service:
	gcc -pthread server.c -o service
