using ScriptAPI;
using System;
using System.Collections.Specialized;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Threading.Tasks;
using System.Globalization; //for time??
// Mathf //round for grid snapping

using static Waypoint; //to get the waypoints you placed
using System.Reflection.PortableExecutable;

public class GhostPathfinding : Script
{

    // [SerializeField]
    // float ManualMovementUnit = 3.0f;

    [SerializeField]
    float moveSpeed = 0.1f;

    [SerializeField]
    bool isMoving = true;
    [SerializeField]
    public bool isChasingPlayer = false;

    [SerializeField]
    public int currRowIndex;
    [SerializeField]
    public int currColIndex;

    public GameObject player;

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
        //Console.WriteLine("Enabled");
        //ExampleAsync();
    }

    public override void Start()
    {
        //Console.WriteLine("Start Test");
        //player = GameObjectScriptFind("FPS_Controller_Script");
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


    public override void Update()
    {
        
        
        
        //Console.WriteLine( GameObjectScriptFind("Waypoint"));
        //Console.WriteLine($"Ghost sees target : ({GameObjectScriptFind("Waypoint").GetComponent<TransformComponent>().GetPosition().X}, {GameObjectScriptFind("Waypoint").GetComponent<TransformComponent>().GetPosition().Z})");
        //Waypoint waypt1 = GameObjectScriptFind("Waypoint").GetComponent<TransformComponent>().GetPosition().Z;
        //^ better way of accessing waypoint
        //List<GameObject> test = MultipleGameObjectScriptFind("Waypoint");
        //MultipleGameObjectScriptFind("Waypoint");
        Vector3 pos = gameObject.GetComponent<TransformComponent>().GetPosition();
        //Console.WriteLine(playerPos.X + " " + playerPos.Y + " " + playerPos.Z);
        //Vector3 playerPos = player.GetComponent<TransformComponent>().GetPosition();
        
       
       

        
        //set MOUSEPOS to be the position where the ghost will go (ghost approaches cabinet in the room)
        //Vector3 wp1 = Input.GetMousePosition(); //seems like there is offset
        //old: 
        Vector3 wp1 = new Vector3(Input.ScreenMousePosdX(), Input.ScreenMousePosdY(), 0);
        //using my own:
        //Vector3 wp1 = new Vector3(Input.GetLocalMousePosX(), Input.GetLocalMousePosY(), 0); 

        //Vector3 wp1 = new Vector3(Input.mousePickedPositionX, Input.mousePickedPositionY, 0); 

        

        float viewportX = (wp1.X + 1.0f) * 0.5f * 1280;
        float viewportY = (1.0f - wp1.Y) * 0.5f * 720; // Corrected for top-left origin

        //do grid snapping
        int gridwidth = 100;
        int gridheight = 100;
        wp1.X = Mathf.Round(viewportX / gridwidth) * gridwidth;
        wp1.Y = Mathf.Round(viewportY / gridheight) * gridheight;

        //how to get the scene window pos?
        //float HardcodeOffsetX = 177.0f; //range = 177 to 1279
        //float HardcodeOffsetZ = 104.0f; //range = 104 to 710
        //wp1.X = wp1.X+HardcodeOffsetX;
        //wp1.Y = wp1.Y+HardcodeOffsetZ;

        //just debug mouse pos first
        
        Console.WriteLine("X: ");
        Console.WriteLine(wp1.X);
        Console.WriteLine("Z: ");
        Console.WriteLine(wp1.Y);


        currRowIndex = (int)wp1.X;
        currColIndex = (int)wp1.Y;


        //move the ghost if its posX and posZ is not = target Pos (target can be either player or the patrol waypoint)
        if (Input.GetMouseButtonDown(Keycode.M1)) //mouse clicked
        {
             if (pos.X != wp1.X)
            {
                if (Math.Abs(pos.X - wp1.X) <= moveSpeed) { pos.X = wp1.X; } //stop moving
                if (pos.X > wp1.X) { pos.X -= moveSpeed; }
                else if (pos.X < wp1.X) { pos.X += moveSpeed; }

            }
            float weirdZpos = wp1.Y;
            if (pos.Z != weirdZpos)
            {

                if (Math.Abs(pos.Z - weirdZpos) <= moveSpeed) { pos.Z = weirdZpos; } //stop
                if (pos.Z > weirdZpos) { pos.Z -= moveSpeed; }
                else if (pos.Z < weirdZpos) { pos.Z += moveSpeed; }

            }
            if (pos.X == wp1.X && pos.Z == weirdZpos)
            {
                
                //done moving
                Console.WriteLine("Done Moving to mouse pos");

            }

        }

        gameObject.GetComponent<TransformComponent>().SetPositionX(pos.X);

        gameObject.GetComponent<TransformComponent>().SetPositionZ(pos.Z);

        //Console.WriteLine($"Enemy Position: ({pos.X}, {pos.Z})");
        //Console.WriteLine($"Going to waypoint at : ({wp1.X}, {wp1.Z})");



    }

    public override void LateUpdate() { }

    public override void OnDisable()
    {
        //Console.WriteLine("Disabled");
    }

    public override void OnDestroy()
    {
        //Console.WriteLine("Exit");
    }


    // Example Usecase (Coroutine Example)
    public async IAsyncEnumerable<int> MyCoroutineAsync()
    {
        for (int i = 0; i < 5; i++)
        {
            //Console.WriteLine("Coroutine step " + i);
            await Task.Delay(1000); // Simulate some work asynchronously
            yield return i;
        }
    }

    // Example Usecase
    async Task<int> ExampleAsync()
    {
        //Console.WriteLine("Starting Unity Coroutine with IEnumerable result");

        await foreach (var value in Coroutine(() => MyCoroutineAsync(), 1000))
        {
            //Console.WriteLine("Yielded Value: " + value);
        }

        //Console.WriteLine("Unity Coroutine finished");

        return 0;
    }
}
