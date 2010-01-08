use warnings;
use strict;
my $chunksize=100;
my @searchkeys=qw(VIA_CONSTRUCAO
VIA_NAO_PAVIMENTADA
VIA_PAV_ASFALTO
VIA_PAV_BLOQUETE
CALCADA
CICLOVIA
);
my %searchkeys;

map {$searchkeys{$_}=1;} @searchkeys;

my $count=1;
sub begin
{
    warn "OUT_${count}.osm";
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

my $id=1;

my %hide;
my %need; # what nodes are needed

begin;
my $interesting=0;
my @data;
sub emit
{

    my $data=join ("\n", @data);
    if (($data =~ /\<way/) &&  ($interesting))
    {
	$id++;

	# emit all the needed points before the way
	foreach my $k (keys %need)
	{
	    if ($hide{$k})
	    {
		print OUT $hide{$k}; #emit the referenced nodes first
#		delete $hide{$k}; it could be referenced twice!
	    }
	}


	print OUT $data;
    }
    else
    {

	if ($data =~ /node/)
	{
	    if ($data =~ /id='(-\d+)'/)
	    {
		$hide{$1}=$data;
	    }
	}
		
    }
    @data=();
    %need=();

    my $rem =$id % $chunksize;
    if ($rem==0)
    {
	warn "$id and $rem $_";
	$id++; # make sure it does not loop 
	end;
	begin;
    }
    $interesting=0;
}

while (<>)
{
    chomp;

    if (/\<\?xml/)
    {
    }
    elsif (/\<osm/)
    {
    }
    elsif (/\/osm/)
    {
    }
    elsif (/\<node/)
    {
	push @data, $_;
    }
    elsif (/\<way/)
    {
	push @data, $_;
    }   
    elsif (/\<nd ref='(-\d+)'/)
    {
	$need{$1}++;
	push @data, $_;
#	print OUT $_;
    }      
    elsif (/\lat=/)
    {
	push @data, $_;
#	print OUT $_;
    }      
    elsif (/\<tag/)
    {
	push @data, $_;
	if (/k='ac:layer_name' v='(\w+)'/)
	{
	    if($searchkeys{$1})
	    {
		$interesting=1;
	    }
	}
	    
#	print OUT $_;
    }      
    elsif (/\<\/node/)
    {
	push @data, $_;
#	print OUT $_ if $interesting;
	emit;
    }
    elsif (/\<\/way/)
    {
#	print OUT $_ if $interesting;
	push @data, $_;
	emit;
    }    
    elsif (/^\s*$/)
    {
    
    }
    else
    {
	warn "$_";
    }
}


end;
