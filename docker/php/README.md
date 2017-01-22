# Docker Apache/PHP 7.0

A Docker image for Apache and PHP 7.0 based on Fedora 25.

## Build the image: 

```
# docker build --rm -t apache .
```

## Start

### Background
To start a container in the background accessible on localhost:8000

```
# docker run -d -p 8000:80 apache
```

### Mount your files
The virtualhost.conf file expose the /src directory. As a default, it only contains an index.php file with a phpinfo(). You can use a volume to overrides this directory:

```
# docker run -d -v /your/php/directory:/src -p 8000:80 apache
```

### Interactive shell

```
# docker exec -i -t apache bash
```

### PHP CLI
To start a container with an interactive shell 

```
# docker run -it -p 8000:80 apache /bin/bash
```

From their, you can use PHP cli

```
# php -v
```

You can also start apache with:

```
# /run-apache.sh
```

After that, use `apachectl -k [stop|start|restart]`

