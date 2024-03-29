<ncl>
  <head>
    <connectorBase>
      <causalConnector id="onBeginStop">
        <simpleCondition role="onBegin"/>
        <simpleAction role="stop"/>
      </causalConnector>
    </connectorBase>
  </head>
  <body>
    <port id="start" component="m"/>
    <media id="m" src="samples/clock.ogv">
      <area id="a1" begin="2s"/>
    </media>
    <media id="img"/>
    <link xconnector="onBeginStop">
      <bind role="onBegin" component="m" interface="a1"/>
      <bind role="stop" component="m"/>
    </link>
  </body>
</ncl>
