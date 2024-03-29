<ncl>
  <head>
    <connectorBase>
      <causalConnector id="onBeginSet">
        <connectorParam name="var"/>
        <connectorParam name="duration"/>
        <simpleCondition role="onBegin"/>
        <simpleAction role="set" value="$var" duration="$duration"/>
      </causalConnector>
    </connectorBase>
  </head>
  <body>
    <port id="start1" component="m1"/>
    <port id="start2" component="m2"/>
    <port id="start3" component="m3"/>
    <media id="m1" src="samples/gnu.png">
      <property name="width" value="50%"/>
      <property name="height" value="50%"/>
      <property name="background" value="red"/>
      <property name="focusIndex" value="0"/>
      <property name="moveUp" value="1"/>
      <property name="moveDown" value="1"/>
      <property name="moveLeft" value="1"/>
      <property name="moveRight" value="1"/>
    </media>
    <media id="m2" src="samples/gnu.png">
      <property name="width" value="50%"/>
      <property name="height" value="50%"/>
      <property name="bottom" value="0"/>
      <property name="right" value="0"/>
      <property name="background" value="green"/>
      <property name="focusIndex" value="1"/>
      <property name="moveUp" value="0"/>
      <property name="moveDown" value="0"/>
      <property name="moveLeft" value="0"/>
      <property name="moveRight" value="0"/>
    </media>
    <media id="m3">
      <area id="a1" begin="1s"/>
      <area id="a2" begin="2s"/>
      <area id="a3" begin="3s"/>
      <area id="a4" begin="4s"/>
    </media>
    <media id="settings" type="application/x-ginga-settings">
      <property name="service.currentFocus" value="1" />
    </media>
    <link xconnector="onBeginSet">
      <bind role="onBegin" component="m3" interface="a1"/>
      <bind role="set" component="settings" interface="service.currentFocus">
        <bindParam name="var" value="0"/>
      </bind>
    </link>
    <link xconnector="onBeginSet">
      <bind role="onBegin" component="m3" interface="a2"/>
      <bind role="set" component="settings" interface="service.currentFocus">
        <bindParam name="var" value="1"/>
      </bind>
    </link>
    <link xconnector="onBeginSet">
      <bind role="onBegin" component="m3" interface="a3"/>
      <bind role="set" component="settings" interface="service.currentFocus">
        <bindParam name="var" value="0"/>
      </bind>
    </link>
    <link xconnector="onBeginSet">
      <bind role="onBegin" component="m3" interface="a4"/>
      <bind role="set" component="settings" interface="service.currentFocus">
        <bindParam name="var" value="1"/>
      </bind>
    </link>
  </body>
</ncl>
