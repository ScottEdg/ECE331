use IO::Socket;
$Socket = IO::Socket::INET->new
(
 Proto 	  => "tcp",
 PeerAddr => "www.wunderground.com",
 PeerPort => 80
);
$Socket->autoflush(1);
print $Socket "GET /\n\r";
while (<$Socket>) {
    print;
}
close $Socket;
