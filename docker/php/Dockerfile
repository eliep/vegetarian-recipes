#
# tt/php
#
FROM fedora:25

RUN echo '%_install_lang all' > /etc/rpm/macros.image-language-conf

# Update distribution
RUN dnf -y update; dnf clean all

# Need Apache in simplest way
RUN dnf -y install httpd; \
    dnf clean all
RUN mkdir -p /var/www/html/www
RUN mkdir -p /var/log/httpd

RUN dnf -y install wget

# Install PHP 7.0
RUN dnf -y install \
  php \
  php-common \
  php-bcmath \
  php-cli \
  php-devel \
  php-mbstring \
  php-mcrypt \
  php-mysqlnd \
  php-pdo \
  php-pear \
  php-pecl-apcu \
  php-pecl-json \
  php-pecl-memcache \
  php-pecl-memcached \
  php-pecl-msgpack \
  php-pecl-mongodb \
  php-pecl-xdebug \
  php-pecl-zip \
  php-php-gettext \
  php-process \
  php-tidy \
  php-intl \
  php-xml;

RUN dnf clean all

# Install phpunit
RUN wget https://phar.phpunit.de/phpunit.phar
RUN cp phpunit.phar /usr/bin/phpunit
RUN chmod a+x /usr/bin/phpunit

# Install composer
RUN curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/bin
RUN cp /usr/bin/composer.phar /usr/bin/composer
RUN chmod a+x /usr/bin/composer

# Copy apache run script
ADD run-apache.sh /run-apache.sh
RUN chmod -v +x /run-apache.sh

# Configure PHP using custom VirtualHost configuration
ADD virtualhost.conf /etc/httpd/conf.d/virtualhost.conf

# Create Apache test page
WORKDIR /src/www
RUN echo "<?php phpinfo();" > /src/www/index.php
RUN rm -rf /var/www/html; ln -s /src /var/www/html

WORKDIR /src

# Done
EXPOSE 80
CMD ["/run-apache.sh"]