local options = {
    backgroundColor="0.6,1.0,0.0",
    ambient="0.1,0.1,0.1",
    gravity="0 -9.8 0",
    botones={
		
		{
			positionX="0.25",
			positionY="0.5",
			texto="Volume",
			nombrePanel="VolumePanel",
			nombreTexto="VolumeTexto",
			tamLetra="0.05",
			material="Azul",
			dimensionX="0.25",
			dimensionY="0.25",
			
			
		},
	}
}

function GetOptions ()
  return options
end