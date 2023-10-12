#!/usr/bin/ruby

require 'socket'
require 'digest'
require 'date'

usage = "usage: #{__FILE__} [[ip] [port]] [file]"
ip    = ARGV[0] or abort usage
port  = ARGV[1] or abort usage
file  = ARGV[2] or abort usage

digest_hexs = []
File.foreach(file) do |line|
  line.chomp!
  digest_hexs << line if line =~ /[a-f0-9]{32}/
end
digest_hexs = digest_hexs.sort.uniq
puts "loaded(#{digest_hexs.length}):", digest_hexs

TCPServer.open ip, port do |server|
  loop do
    puts "waiting for client..."
    client = server.accept
    next_to_crack = digest_hexs.first

    puts "sending: #{next_to_crack}"
    client.puts next_to_crack

    maybe_passwd = client.gets.chomp
    if maybe_passwd != "NOP"
      fmt = "#{next_to_crack}  #{maybe_passwd}\n"
      File.write("#{Date.today}_passwd", fmt, mode: 'a+')
      digest_hexs.delete next_to_crack
      (puts "enough for today :)"; break) if digest_hexs.empty?
    end
    puts "loaded(#{digest_hexs.length}):", digest_hexs

    client.close
  end
end
