<ncl>
  <head>
    <regionBase>
      <region id="r0" top="25%" left="25" width="50%" height="50%">
        <region id="r1" width="50%" height="50%">
          <region id="r" width="50%" height="50%"/>
        </region>
      </region>
    </regionBase>
    <descriptorBase>
      <descriptor id="d" region="r"/>
    </descriptorBase>
  </head>
  <body>
    <port id="p" component="m"/>
    <media id="m" src="samples/arcade.mp3" descriptor="d">
      <property name="background" value="red"/>
    </media>
  </body>
</ncl>
