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
                    position="0 0 0",
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
                    radius="10",
                    trigger="false"
                },
                rigidbody={
                    static="false",
                    velocity="0 -50 -100",
                    density="1000"
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

