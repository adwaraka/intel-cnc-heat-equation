//CnC for Heat Equations

// Tag declarations : <tag-type tag-name>
<pair position>;      		  // position is indexed by the x and y axis of the complex plane

// Item declarations : [item-type item-name <tag-type>] 
[float matrix_value <pair>];      // pixel is indexed by the x and y axis of the complex plane

// Input from the environment and output to the environment
env -> <position>;
env -> [matrix_value];
[matrix_value] -> env;

// Prescriptive relations
<position>::(compute_heat);

[matrix_value: row, col] -> (compute_heat: row, col) -> [matrix_value]; 

