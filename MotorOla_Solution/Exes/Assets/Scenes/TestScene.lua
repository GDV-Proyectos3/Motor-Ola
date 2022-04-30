local level = {
    backgroundColor="0.6,1.0,0.0",
    ambient="0.1,0.1,0.1",
    gravity="0 -9.8 0",
    entidades = {
        {
            name="Pala1",
            id=1,
            components={
                transform={
                    parent="-1",
                    position="100,10,0",
                    rotation="0,0,0",
                    scale="5,5,5"
                },
                mesh={
                    mesh="ogrehead.mesh",
                    material="Ogre/Skin",
                    visible="true"
                }
            }
        },
        {
            name="Ball",
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
                },
                collider={
                    type="sphere",
                    radius="10"
                },
                rigidbody={
                    posX="10",
                    posY="10",
                    posZ="10",
                    oriX="0",
                    oriY="0",
                    oriZ="0",
                    oriW="1"
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

