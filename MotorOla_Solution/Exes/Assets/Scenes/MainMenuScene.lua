local mainMenu = {
    backgroundColor="0.6,1.0,0.0",
    ambient="0.1,0.1,0.1",
    gravity="0 -9.8 0",
    botones={
		
		{
			positionX="0.25",
			positionY="0.75",
			texto="Exit",
			nombrePanel="ExitPanel",
			nombreTexto="ExitTexto",
			tamLetra="0.05",
			material="Azul",
			dimensionX="0.25",
			dimensionY="0.25",
			
			
		},
		{
			positionX="0.25",
			positionY="0.5",
			texto="Options",
			nombrePanel="OptionsPanel",
			nombreTexto="OptionsTexto",
			tamLetra="0.05",
			material="Azul",
			dimensionX="0.25",
			dimensionY="0.25",
			
		},
		{
			positionX="0.25",
			positionY="0.25",
			texto="NewGame",
			nombrePanel="NewGamePanel",
			nombreTexto="NewGameTexto",
			tamLetra="0.05",
			material="Azul",
			dimensionX="0.25",
			dimensionY="0.25",
			
		},
		
		
    }
}

function GetMainMenu ()
  return mainMenu
end

function pruebaTexto()
    print('Im Working YESS')
end