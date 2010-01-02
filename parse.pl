
my $id=0;

print "<osm version='0.6'>\n";
while (<>)
{
    chomp;
    $id--;
    if (/([\-\d\.]+)	([\-\d\.]+) ([\-\d\.]+)/)
    {
	print "<node id='$id' lat='$2' lon='$1' version='1' />\n";	
    }
    else{
	die $_;
    }

}
print "</osm>\n";
