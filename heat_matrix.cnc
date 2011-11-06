//CnC for Heat Equations

// Tag declarations : <tag-type tag-name>
<pair position>;      		  // position is indexed by the x and y axis of the complex plane

// Item declarations : [item-type item-name <tag-type>] 
[float matrix_value <pair>];      // pixel is indexed by the x and y axis of the complex plane

// Prescriptive relations
<position>::(compute_heat);

[matrix_value] -> (compute_heat) -> [matrix_value]; 

// Input from the environment and output to the environment
env -> <position>;
