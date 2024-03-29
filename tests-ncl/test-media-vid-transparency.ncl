<ncl>
  <head>
    <connectorBase>
      <causalConnector id="onBeginSet">
        <connectorParam name="var"/>
        <simpleCondition role="onBegin"/>
        <simpleAction role="set" value="$var" max="unbounded"/>
      </causalConnector>
    </connectorBase>
  </head>
  <body>
    <port id="start-b" component="b"/>
    <media id="b">
      <property name="background" value="pink"/>
    </media>
    <port id="start-m" component="m"/>
    <media id="m" src="samples/clock.ogv">
      <area id="a1" begin="1s"/>
      <area id="a2" begin="2s"/>
      <area id="a3" begin="3s"/>
      <area id="a4" begin="4s"/>
      <area id="a5" begin="5s"/>
      <property name="transparency"/>
    </media>
    <link xconnector="onBeginSet">
      <bind role="onBegin" component="m" interface="a1"/>
      <bind role="set" component="m" interface="transparency">
        <bindParam name="var" value="50%"/>
      </bind>
    </link>
    <link xconnector="onBeginSet">
      <bind role="onBegin" component="m" interface="a2"/>
      <bind role="set" component="m" interface="transparency">
        <bindParam name="var" value="75%"/>
      </bind>
    </link>
    <link xconnector="onBeginSet">
      <bind role="onBegin" component="m" interface="a3"/>
      <bind role="set" component="m" interface="transparency">
        <bindParam name="var" value="255"/>
      </bind>
    </link>
    <link xconnector="onBeginSet">
      <bind role="onBegin" component="m" interface="a4"/>
      <bind role="set" component="m" interface="transparency">
        <bindParam name="var" value="127"/>
      </bind>
    </link>
    <link xconnector="onBeginSet">
      <bind role="onBegin" component="m" interface="a5"/>
      <bind role="set" component="m" interface="transparency">
        <bindParam name="var" value="0%"/>
      </bind>
    </link>
  </body>
</ncl>
