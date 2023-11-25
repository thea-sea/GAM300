using ScriptAPI;
using System;
using System.Collections.Specialized;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Threading.Tasks;
using System.Globalization; //for time??

using static Waypoint; //to get the waypoints you placed

public class GhostMovement : Script
{
    
    // [SerializeField]
    // float ManualMovementUnit = 3.0f;
    
    [SerializeField]
    float patrolSpeed = 0.1f;

    [SerializeField]
    int currTargetWaypt = 0;
    // float angle = 0f;

    [SerializeField]
    float deltaTime = 0f;

    [SerializeField]
    int NumOfWaypoints = 1;

    [SerializeField]
    bool isPatrol = true;
    [SerializeField]
    bool isChasingPlayer = false;

    public override void Awake() 
    {
        // otherEntity = GameObjectScriptFind("Waypoint");
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
     
    }
    public void ReadWaypoint(Waypoint wp)
    {

    }

     // rotate the ghost to look at the target waypoint/player
    void LookAtPoint(Vector3 targetPoint) //WIP (need to finish asap)
    {
        Vector3 upVector = new Vector3(0f, 1f, 0f);
        Vector3 directionToTarget = targetPoint - gameObject.GetComponent<TransformComponent>().GetPosition();
        directionToTarget.Y = 0; //rotation is only around the y-axis

        //Quaternion targetRotation = Quaternion.LookRotation(directionToTarget, upVector);
       
        //gameObject.GetComponent<TransformComponent>().SetRotation(targetRotation);
        //set rotation to targetRotation;
    }

    static int CounterBehaviour(int current, int max)
    {
        

        if (current > max)
        {
            current = 0; // Reset to 1 after reaching the maximum value
        }

        return current;
    }


    public override void Update()
    {
        isChasingPlayer = !isPatrol;//can only be one or another


        List<Vector3> waypointVector = new List<Vector3>();
        //Console.WriteLine( GameObjectScriptFind("Waypoint"));
        //Console.WriteLine($"Ghost sees target : ({GameObjectScriptFind("Waypoint").GetComponent<TransformComponent>().GetPosition().X}, {GameObjectScriptFind("Waypoint").GetComponent<TransformComponent>().GetPosition().Z})");
        //Waypoint waypt1 = GameObjectScriptFind("Waypoint").GetComponent<TransformComponent>().GetPosition().Z;
        //^ better way of accessing waypoint
        //List<GameObject> test = MultipleGameObjectScriptFind("Waypoint");
        //MultipleGameObjectScriptFind("Waypoint");
        Vector3 pos = gameObject.GetComponent<TransformComponent>().GetPosition();
        
        deltaTime += 0.01f; //i need help syncing with game time

        for (int i = 0; i < NumOfWaypoints; ++i)
        {
            //set position where the ghost will go (ghost approaches cabinet in the room)
            Vector3 wp1 = new Vector3( 
                GameObjectScriptFind(i.ToString()).GetComponent<TransformComponent>().GetPosition().X, 
                0, 
                GameObjectScriptFind(i.ToString()).GetComponent<TransformComponent>().GetPosition().Z ); 
            //push back the waypoints
            waypointVector.Add(wp1);

           
        }

        //patrol in loop
        if (isPatrol)
        {

            //move the ghost if its posX and posZ is not = target Pos (target can be either player or the patrol waypoint)
            if (pos.X != waypointVector[currTargetWaypt].X)
            {
                if (Math.Abs(pos.X-waypointVector[currTargetWaypt].X) <= patrolSpeed) { pos.X = waypointVector[currTargetWaypt].X;} //stop moving
                if (pos.X > waypointVector[currTargetWaypt].X) {pos.X -= patrolSpeed;}
                else if (pos.X < waypointVector[currTargetWaypt].X) {pos.X += patrolSpeed;}

            }
            if (pos.Z != waypointVector[currTargetWaypt].Z)
            {

                if (Math.Abs(pos.Z-waypointVector[currTargetWaypt].Z) <= patrolSpeed) { pos.Z = waypointVector[currTargetWaypt].Z;} //stop
                if (pos.Z > waypointVector[currTargetWaypt].Z) {pos.Z -= patrolSpeed;}
                else if (pos.Z < waypointVector[currTargetWaypt].Z) {pos.Z += patrolSpeed;}

            }
            if (pos.X == waypointVector[currTargetWaypt].X && pos.Z == waypointVector[currTargetWaypt].Z)
            {
                currTargetWaypt++; //move to next waypoint
                if (currTargetWaypt == NumOfWaypoints) { currTargetWaypt = 0; }
                
            }
        }
        

        // pos.X = startingX + patrolRadius * (float)Math.Cos(angle);
        // pos.Z = startingZ + patrolRadius * (float)Math.Sin(angle);
        
        gameObject.GetComponent<TransformComponent>().SetPositionX(pos.X);

        gameObject.GetComponent<TransformComponent>().SetPositionZ(pos.Z);

        //Console.WriteLine($"Enemy Position: ({pos.X}, {pos.Z})");
        //Console.WriteLine($"Going to waypoint at : ({wp1.X}, {wp1.Z})");

   
        
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
