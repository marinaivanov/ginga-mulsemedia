<ncl>
  <head>
	<regionBase>
		<region id="regSeek" top="3%" left="91%" width="7%" height="5%" zIndex="2"/>
	</regionBase>

	<descriptorBase>
	<descriptor id="descSeek" region="regSeek"/>
	</descriptorBase>
    <connectorBase>
      <causalConnector id="onKeySelectionSetStop">
        <connectorParam name="var"/>
	<connectorParam name="key"/>
	<simpleCondition role="onSelection" key="$key"/>
        <simpleAction role="set" value="$var"/>
  	<simpleAction role="stop"/>
      </causalConnector>
  </connectorBase>
  </head>
  <body>
    <port id="p1" component="video"/>
    <port id="p2" component="seek"/>
    
    <media id="video" src="samples/animGar.mp4">
      <property name="currentTime"/>	
    </media>
    <media id="seek" src="samples/fastforward.png" descriptor="descSeek"/>
    <link xconnector="onKeySelectionSetStop">
	<bind role="onSelection" component="seek">
	<bindParam name="key" value="GREEN"/>
	</bind>
	<bind role="set" component="video" interface="currentTime">
		<bindParam name="var" value="42s"/>
	</bind>
	<bind role="stop" component="seek"/>
    </link>
  </body>
</ncl>
