<?xml version="1.0" encoding="ISO-8859-1"?>
<ncl id="aplTesteEffect" xmlns="http://www.ncl.org.br/NCL3.0/EDTVProfile">
 <head>
	<regionBase>
		<region id="rg1" width="100%" height="100%" zIndex = "0" />
		<region id="rg2" location="center:top:front" />
		<region id="rg3" width="25%" height="25%" top="55%" left="40%"/>
		<region id="rgFull" width="100%" height="100%"/>

   	</regionBase>

	<descriptorBase>
	    <descriptor id="desc1"  region="rg1" explicitDur="10s"/> 
		<descriptor id="desc2"  region="rg2" intensityValue="100" intensityRange="[0,10]" scent="Coconuts"/>
		<descriptor id="desc3"  region="rg3" explicitDur="3s"/>
		<descriptor id="dsInfo" focusIndex="0"/>
	</descriptorBase>
	
	<connectorBase>

	<causalConnector id="onBeginStart">
		<simpleCondition role="onBegin" />
		<simpleAction role="start" max="unbounded"/>
	</causalConnector>

	<causalConnector id="onBeginSet">
        <connectorParam name="var" />
        <simpleCondition role="onBegin" />
        <simpleAction role="set" value="$var" max="unbounded" />
      </causalConnector>

	<causalConnector id="onEndStop">
		<simpleCondition role="onEnd" />
		<simpleAction role="stop" max="unbounded"/>
	</causalConnector>

	<causalConnector id="onSelectionStart">
        <simpleCondition role="onSelection" key="ENTER"/>
        <simpleAction role="start"/>
	</causalConnector>

	<causalConnector id="onSelectionSet">
        <simpleCondition role="onSelection" key="ENTER"/>
        <simpleAction role="set" value="true"/>
	</causalConnector>
		
	</connectorBase>
  </head>

  <body>
  	
	<port id="pBackground" component="background" />
	
	<media id="background" src="media/img-scent.png" descriptor="desc1">
		<area id="a1" begin="3s" end="5s"/>
	</media>
	<media id="okImage" src="media/ok.png" descriptor="desc3"/>
	
	<effect id="scentEffect" type="ScentType" descriptor="desc2"/>

	<link xconnector="onBeginStart">
		<bind role="onBegin" component="background" interface="a1" />
		<bind role="start" component="scentEffect" />
	</link>

	<link xconnector="onSelectionStart">
		<bind role="onSelection" component="background"/>
		<bind role="start" component="okImage" />
	</link>		

	<link xconnector="onEndStop">
		<bind role="onEnd" component="background" />
		<bind role="stop" component="scentEffect" />
	</link>

	<link xconnector="onEndStop">
		<bind role="onEnd" component="okImage" />
		<bind role="stop" component="scentEffect" />
	</link>
</body>
</ncl>

