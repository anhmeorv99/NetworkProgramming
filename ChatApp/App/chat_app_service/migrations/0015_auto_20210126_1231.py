# Generated by Django 3.1.3 on 2021-01-26 12:31

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('chat_app_service', '0014_chatprivate_message'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='message',
            name='room',
        ),
        migrations.RemoveField(
            model_name='room',
            name='member',
        ),
        migrations.DeleteModel(
            name='ChatPrivate',
        ),
        migrations.DeleteModel(
            name='Message',
        ),
        migrations.DeleteModel(
            name='Room',
        ),
    ]
