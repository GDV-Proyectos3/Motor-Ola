local level = {
  ambient="0.1,0.1,0.1",
  gravity="0 -9.8 0",
  entidades = {
    {
      name="Pala",
      id=1,
      components={
        transform={
          parent="-1"
          position="0,0,0",
          rotation="0,0,0",
          scale="1,1,1",
        },
        renderer={
          mesh="Sinbad.mesh",
          material="",
          visible="true"
        }
      }
    }
  }
}

function GetLevel ()
  return level
end

