my $count=1;
sub begin
{
    open OUT , ">OUT_${count}.osm";
    $count++;
    print OUT '<?xml version="1.0" encoding="UTF-8"?>' . "\n";
    print OUT '<osm version="0.6" generator="JOSM">' . "\n";
}

sub end 
{
    print OUT '</osm>' . "\n";    
    close OUT;
}

my $id=0;

begin;

sub emit
{
    my $rem =$id % 100000;
    if ($rem==0)
    {
	warn "$id and $rem $_";
	end;
	begin;
	next;
    }
}

while (<>)
{

    if (/\<xml /)
    {
    }
    elsif (/\osm/)
    {
    }
    elsif (/\<node/)
    {
	$id++;
	print OUT $_;
    }
    elsif (/\<way/)
    {
	$id++;
	print OUT $_;
    }   
    elsif (/\<nd/)
    {
	print OUT $_;
    }      
    elsif (/\lat=/)
    {
	print OUT $_;
    }      
    elsif (/\<tag/)
    {
	print OUT $_;
    }      
    elsif (/\<\/node/)
    {
	print OUT $_;
	emit;
    }
    elsif (/\<\/way/)
    {
	print OUT $_;
	emit;
    }    
    else
    {
	warn "$_";
    }
}


end;
