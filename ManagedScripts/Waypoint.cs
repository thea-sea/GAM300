using ScriptAPI;
using System;
using System.Collections.Specialized;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Threading.Tasks;
using System.Globalization; //for time



public class Waypoint : Script
{
    
      [SerializeField]
      float runtimePosx = 0f;
      [SerializeField]
      float runtimePosz = 0f;

    public float waypointX 
    {
        get { return runtimePosx; /*gameObject.GetComponent<TransformComponent>().GetPosition().X;*/ }
        set { runtimePosx = value; }
    }
  
    public float waypointZ 
    {
        get { return runtimePosz; }
        set { runtimePosz = value; }
    }

    public override void Awake() 
    {
        //otherEntity = GameObjectScriptFind("entity3");
        //Console.WriteLine(otherEntity);
        //otherEntity.GetComponent<TransformComponent>().SetPositionX(20);
        //Console.WriteLine(script.gameObject);
        //gameObject.GetTransformComponent();
    }
   

    public override void OnEnable() 
    {
        Console.WriteLine("Enabled");
        ExampleAsync();
    }

    public override void Start()
    {
        Console.WriteLine("Start Test");
         Vector3 pos = gameObject.GetComponent<TransformComponent>().GetPosition();
          waypointX = pos.X;
          waypointZ = pos.Z;
         gameObject.GetComponent<TransformComponent>().SetPositionX(pos.X);
         gameObject.GetComponent<TransformComponent>().SetPositionZ(pos.Z);
       // Console.WriteLine($"POS : ({pos.X}, {pos.Z})");


    }

    public override void Update()
    {
        Vector3 pos = gameObject.GetComponent<TransformComponent>().GetPosition();
        //Console.WriteLine($"POS : ({pos.X}, {pos.Z})");

        gameObject.GetComponent<TransformComponent>().SetPositionX(pos.X);
        gameObject.GetComponent<TransformComponent>().SetPositionZ(pos.Z);
        waypointX = pos.X;
        waypointZ = pos.Z;
    }

    public override void LateUpdate() { }

    public override void OnDisable() 
    {
        Console.WriteLine("Disabled");
    }

    public override void OnDestroy() 
    {
        Console.WriteLine("Exit");
    }


    // Example Usecase (Coroutine Example)
    public async IAsyncEnumerable<int> MyCoroutineAsync()
    {
        for (int i = 0; i < 5; i++)
        {
            Console.WriteLine("Coroutine step " + i);
            await Task.Delay(1000); // Simulate some work asynchronously
            yield return i;
        }
    }

    // Example Usecase
    async Task<int> ExampleAsync()
    {
        Console.WriteLine("Starting Unity Coroutine with IEnumerable result");

        await foreach (var value in Coroutine(() => MyCoroutineAsync(), 1000))
        {
            Console.WriteLine("Yielded Value: " + value);
        }

        Console.WriteLine("Unity Coroutine finished");

        return 0;
    }
}
