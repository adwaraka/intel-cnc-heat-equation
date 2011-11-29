//CnC for Heat Equations

// Tag declarations : <tag-type tag-name>
<MatrixKey position>;      		  // position is indexed by the x,y and t axis of the complex plane

// Item declarations : [item-type item-name <tag-type>] 
[float matrix_value <MatrixKey>];      // pixel is indexed by the x,y and t axis of the complex plane

// Prescriptive relations
<position>::(compute_heat);

// Input from the environment and output to the environment
env -> <position>;
env -> [matrix_value];
[matrix_value] -> env;

[matrix_value] -> (compute_heat);
(compute_heat) -> [matrix_value]; 

