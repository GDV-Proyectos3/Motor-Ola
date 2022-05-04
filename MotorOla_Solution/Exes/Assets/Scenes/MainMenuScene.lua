local mainMenu = {
    backgroundColor="0.6,1.0,0.0",
    ambient="0.1,0.1,0.1",
    gravity="0 -9.8 0",
    botones={
		{
			positionX="0.10",
			positionY="0.375",
			texto="Play",
			nombrePanel="NewGamePanel",
			nombreTexto="NewGameTexto",
			tamLetra="0.05",
			material="LapidaButton",
			dimensionX="0.25",
			dimensionY="0.35",
		},
		{
			positionX="0.375",
			positionY="0.375",
			texto="Options",
			nombrePanel="OptionsPanel",
			nombreTexto="OptionsTexto",
			tamLetra="0.05",
			material="LapidaButton",
			dimensionX="0.25",
			dimensionY="0.35",
		},
		{
			positionX="0.65",
			positionY="0.375",
			texto="Exit",
			nombrePanel="ExitPanel",
			nombreTexto="ExitTexto",
			tamLetra="0.05",
			material="LapidaButton",
			dimensionX="0.25",
			dimensionY="0.35",
		},
    }
}

function GetMainMenu ()
  return mainMenu
end

function pruebaTexto()
    print('Im Working YESS')
end