#!/bin/sh
export HOME=/home/ps0ke
export PATH=/package/host/localhost/ruby-2.1.1/bin:$PATH
export PATH=$HOME/.gem/ruby/2.1.1/bin:$PATH
exec ruby db-irc-notify.rb
