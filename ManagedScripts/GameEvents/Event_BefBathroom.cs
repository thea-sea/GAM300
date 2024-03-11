﻿using ScriptAPI;
using System;

public class EventBefBathroom : Script
{
    public GameObject? door;

    public override void Update()
    {
        if (door != null)
        {
            if (door.GetComponent<Door_Script>().locked == false)
            {
                Console.WriteLine("Martin (Internal): The tub is still wet, but there’s no one...");
            }
        }
    }

    public override void OnTriggerEnter(ColliderComponent collider)
    {
        Console.WriteLine("Martin (Internal): The shower’s running... but I don’t hear anyone in there.");
        gameObject.GetComponent<ColliderComponent>().SetEnabled(false);
    }
}