#!/usr/bin/csh
foreach l ( a b c d e f g h i j k l m n o p q r s t u v w x y z )
  set files=" `ls /shrunis/ipf/src/ipf/incref/$l*.inc` "
  foreach file ( $files )
    cp ${file} .
  end
  set files=" `ls /shrunis/ipf/src/ipf/incref/$l*.h` "
  foreach file ( $files )
    cp ${file} .
  end
  set files=" `ls /shrunis/ipf/src/ipf/srcref/$l*.[fch]` "
  foreach file ( $files )
    cp ${file} .
  end
end
exit
