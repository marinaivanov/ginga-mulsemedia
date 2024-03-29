<ncl>
  <head>
    <regionBase>
      <region id="reg" width="100%" height="100%"/>
    </regionBase>
    <descriptorBase>
      <descriptor id="desc" region="reg"/>
    </descriptorBase>
    <connectorBase>
      <causalConnector id="onEndStart">
        <simpleCondition role="onEnd"/>
        <simpleAction role="start"/>
      </causalConnector>
    </connectorBase>
  </head>
  <body>
    <port id="start" component="m"/>
    <media id="m" src="samples/vector.svg" descriptor="desc"/>
    <link xconnector="onEndStart">
      <bind role="onEnd" component="m"/>
      <bind role="start" component="m"/>
    </link>
  </body>
</ncl>
