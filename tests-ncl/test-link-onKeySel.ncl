<ncl>
  <head>
  
     <regionBase>
      <region id="reg" width="50%" height="50%"/>
      <region id="reg2" top="50%" width="50%" height="50%"/>
    </regionBase>
    <descriptorBase>
      <descriptor id="desc" region="reg"/>
      <descriptor id="desc2" region="reg2"/>
    </descriptorBase>
  
  
    <connectorBase>
      <causalConnector id="onSelectionSet">
        <connectorParam name="key"/>
        <connectorParam name="var"/>
        <simpleCondition role="onSelection" key="$key" user = "$user"/>
        <simpleAction role="set" value="$var"/>
      </causalConnector>
     
  
       <causalConnector id="onVoiceRecognitionStart">
        <connectorParam name="key"/>
        <connectorParam name="user"/>
         <simpleCondition role="onVoiceRecognition" key="$key"/>
         <simpleAction role="start" />
      </causalConnector>  
  
  
      
    </connectorBase>
  </head>
  <body>
    <port id="start" component="time"/>
    <media id="time">
      <property name="background" value="white"/>
      <property name="bounds" value="25%,25%,50%,50%"/>
    </media>
    
    
    <media id="falou" src="images/askRed.png" descriptor="desc2" />
    
    
    <link xconnector="onSelectionSet">
      <bind role="onSelection" component="time">
        <bindParam name="key" value="RED"/>
      </bind>
      <bind role="set" component="time" interface="background">
        <bindParam name="var" value="red"/>
      </bind>
    </link>
    <link xconnector="onSelectionSet">
      <bind role="onSelection" component="time">
        <bindParam name="key" value="GREEN"/>
      </bind>
      <bind role="set" component="time" interface="background">
        <bindParam name="var" value="green"/>
      </bind>
    </link>
    <link xconnector="onSelectionSet">
      <bind role="onSelection" component="time">
        <bindParam name="key" value="BLUE"/>
      </bind>
      <bind role="set" component="time" interface="background">
        <bindParam name="var" value="blue"/>
      </bind>
    </link>
    <link xconnector="onSelectionSet">
      <bind role="onSelection" component="time">
        <bindParam name="key" value="YELLOW"/>
      </bind>
      <bind role="set" component="time" interface="background">
        <bindParam name="var" value="yellow"/>
      </bind>
    </link>


 
  <link xconnector="onVoiceRecognitionStart">
      <bind role="onVoiceRecognition" component="time">
        <bindParam name="key" value="RED"/>
        <bindParam name="user" value="FABIO"/>
      </bind>
      <bind role="start" component="falou"  />
    </link>
    
    
    
    
  </body>
</ncl>
