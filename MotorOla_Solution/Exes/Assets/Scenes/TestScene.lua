local level = {
    backgroundColor="0.9,0.0,0.1",
    ambient="0.1,0.1,0.1",
    gravity="0 -9.8 0",
    camera = {
        nearClipDistance="1",
        farClipDistance="10000",
        camPosition="0,0,-1000",
        camRotation="0,0,0"
    },
    entidades = {
        {
            name="CentroEscena",
            id=1,
            components={
                transform={
                    parent="-1",
                    position="0,0,0",
                    rotation="0,0,0",
                    scale="5,5,5"
                },
                mesh={
                    mesh="ogrehead.mesh",
                    material="Verde",
                    visible="true"
                }
            }
        },
        {
            name="Pala2",
            id=2,
            components={
                transform={
                    parent="-1",
                    position="0,0,0",
                    rotation="0,0,0",
                    scale="1,1,1"
                },
                mesh={
                    mesh="cube.mesh",
                    material="Rojo",
                    visible="true"
                }
            }
        }
    }
}

function GetLevel ()
  return level
end

function pruebaTexto()
    print('Im Working YESS')
end

