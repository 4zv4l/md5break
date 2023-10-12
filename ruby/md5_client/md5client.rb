#!/usr/bin/ruby

require 'socket'
require 'digest'

usage = "usage: #{__FILE__} [[ip] [port]] [file]"
ip    = ARGV[0] or abort usage
port  = ARGV[1] or abort usage
file  = ARGV[2] or abort usage

TCPSocket.open ip, port do |server|
  digest = server.gets.chomp
  puts "Recv: #{digest}"

  File.foreach file do |password|
    password.chomp!
    found = Digest::MD5.hexdigest(password)

    if digest == found
      server.puts password
      return puts "Send: #{password}"
    end
  end

  server.puts "NOP"
  return puts "Send: NOP"
end

# 5d41402abc4b2a76b9719d911017c592 hello
