local level = {
    backgroundColor="0.6,1.0,0.0",
    ambient="0.1,0.1,0.1",
    gravity="0.0,-9.8,0.0",
    camera = {
        nearClipDistance="1",
        farClipDistance="10000",
        camPosition="0,0,0",
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
                    material="Azul",
                    visible="true"
                },
				
            }
        },
        {
            name="Este",
            id=2,
            components={
                transform={
                    parent="-1",
                    position="1000,0,0",
                    rotation="0,0,0",
                    scale="1,1,1"
                },
                mesh={
                    mesh="cube.mesh",
                    material="Rojo",
                    visible="true"
                },
				
            }
        },
        {
            name="Oeste",
            id=3,
            components={
                transform={
                    parent="-1",
                    position="-1000,0,0",
                    rotation="0,0,0",
                    scale="1,1,1"
                },
                mesh={
                    mesh="cube.mesh",
                    material="Rojo",
                    visible="true"
                }
            }
        },
        {
            name="Arriba",
            id=4,
            components={
                transform={
                    parent="-1",
                    position="0,1000,0",
                    rotation="0,0,0",
                    scale="1,1,1"
                },
                mesh={
                    mesh="cube.mesh",
                    material="Rojo",
                    visible="true"
                }
            }
        },
        {
            name="Abajo",
            id=5,
            components={
                transform={
                    parent="-1",
                    position="0,-1000,0",
                    rotation="0,0,0",
                    scale="1,1,1"
                },
                mesh={
                    mesh="cube.mesh",
                    material="Azul",
                    visible="false"
                }
            }
        },
        {
            name="Sur",
            id=6,
            components={
                transform={
                    parent="-1",
                    position="0,0,1000",
                    rotation="0,0,0",
                    scale="1,1,1"
                },
                mesh={
                    mesh="cube.mesh",
                    material="Rojo",
                    visible="true"
                }
            }
        },
        {
            name="Norte",
            id=7,
            components={
                transform={
                    parent="-1",
                    position="0,0,-1000",
                    rotation="0,0,0",
                    scale="1,1,1"
                },
                mesh={
                    mesh="cube.mesh",
                    material="Rojo",
                    visible="true"
                }
            }
        },
        {
            name="Ball",
            id=8,
            components={
                transform={
                    parent="-1",
                    position="500 0 0",
                    rotation="0,0,0",
                    scale="0.8,0.8,0.8" -- /!\ deberían ser los tres iguales (más cómodo y se preserva la forma esférica)
                },
                mesh={
                    mesh="sphere.mesh",
                    material="Cian",
                    visible="true"
                },  
                collider={
                    type="sphere",
                    radius="80", -- /!\ = escala * 100; (podría ajustarse automáticamente)
                    trigger="false"
                },
                rigidbody={
                    static="false",
                    -- velocity="-100 0 0",
                    density="1000"
                }
            }
        },
        {
            name="Ball2",
            id=9,
            components={
                transform={
                    parent="-1",
                    position="500 -1000 0",
                    rotation="0,0,0",
                    scale="1.5,1.5,1.5" -- /!\ deberían ser los tres iguales (más cómodo y se preserva la forma esférica)
                },
                mesh={
                    mesh="sphere.mesh",
                    material="Rosa",
                    visible="true"
                },  
                collider={
                    type="sphere",
                    radius="150", -- /!\ = escala * 100; (podría ajustarse automáticamente)
                    trigger="false"
                },
                rigidbody={
                    static="true"
                }
            }
        },
        {
            name="Cube",
            id=10,
            components={
                transform={
                    parent="-1",
                    position="0 0 0",
                    rotation="0,0,0",
                    scale="1,1,2" -- /!\ si son los tres iguales se preserva la forma cúbica
                },
                mesh={
                    mesh="cube.mesh",
                    material="Cian",
                    visible="true"
                },  
                collider={
                    type="box",
                    x="50", -- /!\ = escalaEnX * 50; (podría ajustarse automáticamente)
                    y="50", -- "    escalaEnY "
                    z="100", -- "   escalaEnZ "
                    trigger="false"
                },
                rigidbody={
                    static="false",
                    -- velocity="-100 0 0",
                    density="1000"
                }
            }
        },
        {
            name="Cube2",
            id=11,
            components={
                transform={
                    parent="-1",
                    position="0 -1000 0",
                    rotation="0,0,0",
                    scale="0.5,1.8,1" -- /!\ si son los tres iguales se preserva la forma cúbica
                },
                mesh={
                    mesh="cube.mesh",
                    material="Rosa",
                    visible="true"
                },  
                collider={
                    type="box",
                    x="25", -- /!\ = escalaEnX * 50; (podría ajustarse automáticamente)
                    y="90", -- "   escalaEnY "
                    z="50", -- "   escalaEnZ "
                    trigger="false"
                },
                rigidbody={
                    static="true"
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

