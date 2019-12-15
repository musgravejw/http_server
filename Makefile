target:
	@mkdir -p dist
	@gcc src/server.c -w -o dist/server 

clean:
	@rm -rf dist
