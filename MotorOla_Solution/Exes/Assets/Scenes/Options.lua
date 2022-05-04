local options = {
    backgroundColor="0.6,1.0,0.0",
    ambient="0.1,0.1,0.1",
    gravity="0 -9.8 0",
    botones={
		{
			positionX="0.2",
			positionY="0.2",
			texto="",
			nombrePanel="VolumePanel",
			nombreTexto="VolumeTexto",
			tamLetra="0.1",
			material="AltavozButton",
			dimensionX="0.25",
			dimensionY="0.25",
		},
		{
			positionX="0.55",
			positionY="0.15",
			texto="Return",
			nombrePanel="ReturnPanel",
			nombreTexto="ReturnTexto",
			tamLetra="0.05",
			material="LapidaButton",
			dimensionX="0.25",
			dimensionY="0.35",
		},
	}
}

function GetOptions ()
  return options
end