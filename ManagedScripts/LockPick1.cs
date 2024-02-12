using System;
using System.ComponentModel.DataAnnotations;
using ScriptAPI;

public class LockPick1 : Script
{
    float toDegree(float radians)
    {
        return radians * (180 / 3.1415926535897931f);
    }
    float toRadians(float degree)
    {
        return degree * (3.1415926535897931f / 180);
    }

    //[Header("Tutorial UI Variables")]
    //public Image _TutorialImage;
    //public Sprite[] _TutorialImgSprites;
    //public Text _Press2Continue;
    //[SerializeField] int _TutorialStep;

    //public Text mySubtitles;
    //public Image mySubtitlesBG;
    String[] Subtitles;
    [SerializeField]
    public static int counter;

    public AudioSource myVOSource;
    public bool _TutorialCompleted;

    [Header("Lockpick Variables")]
    public GameDataManager myGameDataManager;
    public GameObject Door_UI;
    public CameraComponent cam;
    public CameraComponent playerCam;
    public TransformComponent innerLock;
    public TransformComponent pickPosition;
    public GameObject playerController;
    public GameObject lockGroup;
    public string difficultyLvl;
    public float maxAngle = 90;
    public float lockSpeed = 10;
    public bool unlocked;
    //public AudioClip[] lockSoundEffects;
    //public AudioClip[] rattleSoundEffects;
    public String[] lockSoundEffects;
    public String[] rattleSoundEffects;
    public String[] playerGuideVO;
    float delay = 0.4f;
    public GameObject _NumOfTries;
    public UISpriteComponent _AmtOfTries;

    [Range(1, 25)]
    public float lockRange = 10;

    [SerializeField] private int numOfTries;
    private float percentage;
    private float eulerAngle;
    private float unlockAngle;
    private Vector2 unlockRange;
    private float keyPressTime;
    private bool movePick;
    private bool deduct;
    private bool displayTutorial;
    [SerializeField] bool played;

    private Vector3 originalPosition = new Vector3(0.0f, 350.0f, 1500.0f);
    private Vector3 originalRotation;
    AudioComponent audio;
    bool failed;
    bool passed;
    float timer;
    
    public int doorIndex;
    public GameObject doorText;
    public GameObject monster;
    public GameObject doorStates;
    public GameObject popupUI;

    // Start is called before the first frame update
    override public void Awake()
    {
        lockSoundEffects = new String[3];
        lockSoundEffects[0] = "Lock Turning Audio";
        lockSoundEffects[1] = "lockpick success";
        lockSoundEffects[2] = "lockpick_failtryl";

        rattleSoundEffects = new String[6];
        rattleSoundEffects[0] = "temp_lockrattle1";
        rattleSoundEffects[1] = "temp_lockrattle2";
        rattleSoundEffects[2] = "temp_lockrattle3";
        rattleSoundEffects[3] = "temp_lockrattle4";
        rattleSoundEffects[4] = "temp_lockrattle5";
        rattleSoundEffects[5] = "temp_lockrattle6";

        playerGuideVO = new String[7];
        playerGuideVO[0] = "pc_lockpickstart";
        playerGuideVO[1] = "pc_lockpicksuccess1";
        playerGuideVO[2] = "pc_lockpicksuccess2";
        playerGuideVO[3] = "pc_lockpickfail";
        playerGuideVO[4] = "";
        playerGuideVO[5] = "";
        playerGuideVO[6] = "";

        Subtitles = new String[7];
        Subtitles[0] = "Martin (Internal): Hopefully, I won\'t forget how to do this.";
        Subtitles[1] = "Martin (Internal): Alright, looks like I\'m in.";
        Subtitles[2] = "Martin (Internal): No turning back now.";
        Subtitles[3] = "Martin (Internal): That was too loud... I better not screw up again.";
        Subtitles[4] = "";
        Subtitles[5] = "Move [mouse] to adjust pick";
        Subtitles[6] = "Press [E] to turn lock";

        counter = 0;
        audio = gameObject.GetComponent<AudioComponent>();

        newLock();
    }

    // Update is called once per frame
    override public void Update()
    {
        Input.Lock(false);
        doorText.SetActive(false);

        //if (!_TutorialCompleted)
        //{
        //    // NOTE: Audio
        //    //if (!myVOSource.isPlaying() && !played)
        //    //{
        //    //    myVOSource.Play();
        //    //    mySubtitlesBG.enabled = true;
        //    //    mySubtitles.text = "Martin (Internal): Hopefully, I won’t forget how to do this.";
        //    //    played = true;

        //    //}
        //    //else if (!myVOSource.isPlaying() && played)
        //    //{
        //    //    mySubtitles.text = "";
        //    //    mySubtitlesBG.enabled = false;
        //    //    _NumOfTries.SetActive(true);
        //    //    movePick = true;
        //    //    _TutorialCompleted = true;
        //    //}
        //}
        //else
        //{
        //_NumOfTries.SetActive(true);

        //subtitles
        UISpriteComponent Sprite = GameObjectScriptFind("VOSubtitles").GetComponent<UISpriteComponent>();
        //UISpriteComponent Sprite = gameObject.GetComponent<UISpriteComponent>();
        Sprite.SetFontMessage(Subtitles[counter]);

        if (counter < 5)
        audio.play(playerGuideVO[counter]);

        #region Move Pick
        if (movePick)
            {
                
                //Vector3 dir = Input.mousePosition - cam.WorldToScreenPoint(transform.position);
                Vector3 dir = Input.GetLocalMousePos() - new Vector3(Screen.width / 2, Screen.height / 2, 0);  //cam.WorldToScreenPoint(transform.GetPosition());

                //eulerAngle = Vector3.Angle(dir, Vector3.Up());
                eulerAngle = Vector3.Angle(dir, Vector3.Down());

                Vector3 cross = Vector3.Cross(Vector3.Up(), dir);
                if (cross.Z < 0) { eulerAngle = -eulerAngle; }
                eulerAngle = Mathf.Clamp(eulerAngle, toRadians(-maxAngle), toRadians(maxAngle));

                Quaternion rotateTo = Quaternion.AngleAxis(eulerAngle, Vector3.Forward());
                //transform.SetRotation(Quaternion.EulerAngle(rotateTo));

                Vector3 originalRotation = transform.GetRotation();
                transform.SetRotation(new Vector3(originalRotation.X, originalRotation.Y, -eulerAngle));

                Vector2 newPosition = new Vector2(originalPosition.X * Mathf.Cos(-eulerAngle) - originalPosition.Y * Mathf.Sin(-eulerAngle),
                                                  originalPosition.X * Mathf.Sin(-eulerAngle) + originalPosition.Y * Mathf.Cos(-eulerAngle));
                transform.SetPosition(new Vector3(newPosition.X, newPosition.Y, originalPosition.Z));
            }

            if (!failed && Input.GetKeyDown(Keycode.E))
            {
                originalRotation = transform.GetRotation();
                movePick = false;
                keyPressTime = 1;
                audio.play(lockSoundEffects[0]);
                counter = 5;
            }
            if (Input.GetKeyUp(Keycode.E))
            {
                movePick = true;
                keyPressTime = 0;
                deduct = true;
                if (audio.finished(playerGuideVO[0]))
                counter = 6;
            }
            #endregion

        #region Check if pick is at correct position
        float eulerAngleDegree = toDegree(eulerAngle);
        percentage = Mathf.Round(100 - Mathf.Abs(((eulerAngleDegree - unlockAngle) / 100) * 100));
        float maxRotation = (percentage / 100) * maxAngle;
        float lockRotation = maxRotation * keyPressTime;
        float lockLerp = Mathf.LerpAngle(toDegree(innerLock.GetRotation().Z), lockRotation, Time.deltaTime * lockSpeed);
        innerLock.SetRotation(new Vector3(0, 0, toRadians(lockLerp)));

        if (!movePick && (lockLerp >= maxRotation - 1))
        {
            if (eulerAngleDegree < unlockRange.Y && eulerAngleDegree > unlockRange.X)
            {
                audio.stop(lockSoundEffects[0]);
                movePick = true;
                keyPressTime = 0;
                // NOTE: Audio
                audio.play(lockSoundEffects[1]);
                timer = 1.2f;
                passed = true;
                
            }
            else
            {
                //float randomRotation = ScriptAPI.Random.insideUnitCircle.x;
                float randomRotation = ScriptAPI.Random.Range(-1,1); // NOTE: Not sure if insideUnitCircle keeps changing or is a fixed Vector2 that is reset on Start
                transform.SetRotationZ(originalRotation.Z + (float)(randomRotation / 180.0 * Math.PI));

                if (Input.GetKeyDown(Keycode.E) || Input.GetKey(Keycode.E))
                {
                    // NOTE: Audio
                    if (audio.finished(lockSoundEffects[0]))
                    {
                        audio.stop(lockSoundEffects[0]);
                        delay -= Time.deltaTime;

                        if (delay <= 0)
                        {
                            // Not sure if there is a better way to do this
                            if (audio.finished(rattleSoundEffects[0]))
                                audio.stop(rattleSoundEffects[0]);
                            if (audio.finished(rattleSoundEffects[1]))
                                audio.stop(rattleSoundEffects[1]);
                            if (audio.finished(rattleSoundEffects[2]))
                                audio.stop(rattleSoundEffects[2]);
                            if (audio.finished(rattleSoundEffects[3]))
                                audio.stop(rattleSoundEffects[3]);
                            if (audio.finished(rattleSoundEffects[4]))
                                audio.stop(rattleSoundEffects[4]);
                            if (audio.finished(rattleSoundEffects[5]))
                                audio.stop(rattleSoundEffects[5]);

                            audio.play(rattleSoundEffects[(int)ScriptAPI.Random.Range(0, 5)]);
                            delay = 0.4f;
                        }
                    }
                }

                if (deduct == true)
                {
                    numOfTries -= 1;
                    deduct = false;
                }

                if (numOfTries <= 0)
                {
                    // NOTE: Audio
                    audio.play(lockSoundEffects[2]);
                    movePick = false;
                    timer = 1.0f;
                    failed = true;
                }
            }
        }
        #endregion

        _AmtOfTries.SetFontMessage("Number of tries left: " + numOfTries.ToString());

        if (numOfTries <= 1)
        {
            _AmtOfTries.SetFontColour(new Vector4(1.0f, 0.0f, 0.0f, 1.0f)); // red
        }

        if (passed)
        {
            counter = 1;
            if (timer <= 0)
            {
                playerController.SetActive(true);
                Vector3 rotation = playerController.transform.GetRotation();
                Quaternion quat = new Quaternion(rotation);
                Vector3 rotationToVector = new Vector3(-Mathf.Sin(toRadians(rotation.Y)), 0.0f, Mathf.Cos(toRadians(rotation.Y))) * 200;
                Console.WriteLine(rotationToVector.X + "\t" + rotationToVector.Y + "\t" + rotationToVector.Z);
                playerController.GetComponent<RigidBodyComponent>().SetPositionRotationAndVelocity(playerController.transform.GetPosition() + rotationToVector, new Vector4(quat.X, quat.Y, quat.Z, quat.W), new Vector3(1, 1, 1).Normalize(), new Vector3(1, 1, 1).Normalize());
                Input.Lock(true);
                //    playerCam.SetEnabled(true);
                //    Door_UI.SetActive(false);
                unlocked = true;
                doorStates.SetActive(true);
                doorStates.GetComponent<DoorState>().Doors[doorIndex] = DoorState.State.Unlocked;
                lockGroup.SetActive(false);
                newLock();
                GraphicsManagerWrapper.ToggleViewFrom2D(false);
                popupUI.GetComponent<PopupUI>().lockpickDisplayed = false;
            }
            else
            {
                timer -= Time.deltaTime;
            }
            if (audio.finished(playerGuideVO[1]))
            {
            counter = 2;
            }
        }

        if (failed)
        {
            counter = 3;
            if (timer <= 0)
            {
                playerController.SetActive(true);
                Input.Lock(true);
                //    playerCam.SetEnabled(true);
                //    Door_UI.SetActive(false);
                lockGroup.SetActive(false);
                newLock();
                GraphicsManagerWrapper.ToggleViewFrom2D(false);
                doorStates.SetActive(true);
                monster.GetComponent<GhostMovement>().AlertMonster(doorIndex);
                popupUI.GetComponent<PopupUI>().lockpickDisplayed = false;
            }
            else
            {
                timer -= Time.deltaTime;
            }
        }
        //}
    }

    public void newLock()
    {
        originalPosition = new Vector3(0.0f, 600.0f, 2500.0f);
        originalRotation = transform.GetRotation();

        audio.stop(lockSoundEffects[1]);
        audio.stop(lockSoundEffects[2]);
        popupUI.GetComponent<PopupUI>().lockpickDisplayed = true;
        Input.Lock(false);

        failed = false;
        passed = false;

        //Door_UI.SetActive(true);
        _AmtOfTries.SetFontColour(new Vector4(1.0f, 1.0f, 1.0f, 1.0f)); // white
        keyPressTime = 0;
        unlocked = false;
        deduct = true;
        //playerController.SetEnabled(false);
        //playerCam.SetEnabled(false);
        //cam.transform.SetRotation(new Vector3(0, 0, 0));
        unlockAngle = ScriptAPI.Random.Range(-maxAngle + lockRange, maxAngle - lockRange);
        unlockRange = new Vector2(unlockAngle - lockRange, unlockAngle + lockRange);

        numOfTries = 5;
        if (difficultyLvl == "Easy")
        {
            numOfTries = 10;
        }
        else if (difficultyLvl == "Normal")
        {
            numOfTries = 5;
        }
        else if (difficultyLvl == "Hard")
        {
            numOfTries = 3;
        }

        if (_TutorialCompleted)
        {
            movePick = true;
        }
    }
}
